#include <tools/gmd/gmd.hpp>
#include <tools/gmd/helpers.hpp>
#include <zipper.h>
#include <unzipper.h>
#undef snprintf
#include "ZlibHelper.hpp"
#include <external/json.hpp>

USE_BGD_NAMESPACE();

using namespace gmd;
using namespace zipper;
namespace fs = std::filesystem;

constexpr const char* bgd::gmd::GmdTypeToString(GmdType type) {
    switch (type) {
        case kGmdTypeGmd:   return "gmd";
        case kGmdTypeGmd2:  return "gmd2";
        case kGmdTypeLvl:   return "lvl";
        default:            return "gmd";
    }
}

constexpr const wchar_t* bgd::gmd::GmdTypeToWString(GmdType type) {
    switch (type) {
        case kGmdTypeGmd:   return L"gmd";
        case kGmdTypeGmd2:  return L"gmd2";
        case kGmdTypeLvl:   return L"lvl";
        default:            return L"gmd";
    }
}

bool bgd::gmd::isLevelFileName(std::string const& fname) {
    return (
        bgd::string_ends_with(fname, GmdTypeToString(kGmdTypeGmd)) ||
        bgd::string_ends_with(fname, GmdTypeToString(kGmdTypeGmd2)) ||
        bgd::string_ends_with(fname, GmdTypeToString(kGmdTypeLvl))
    );
}

bool bgd::gmd::isLevelFileName(std::wstring const& fname) {
    return (
        bgd::string_ends_with(fname, GmdTypeToWString(kGmdTypeGmd)) ||
        bgd::string_ends_with(fname, GmdTypeToWString(kGmdTypeGmd2)) ||
        bgd::string_ends_with(fname, GmdTypeToWString(kGmdTypeLvl))
    );
}

byte_array bgd::gmd::convert_vs(std::string const& data) {
    return byte_array(data.begin(), data.end());
}

std::string bgd::gmd::convert_vs(byte_array const& data) {
    return std::string(data.begin(), data.end());
}

GmdFile::GmdFile(GJGameLevel* lvl) {
    this->m_pLevel = lvl;
    this->m_sFileName = string_convert(lvl->m_sLevelName);
}

GmdFile::GmdFile(std::string const& pathStr) {
    auto path = fs::path(pathStr);
    if (fs::is_directory(path)) {
        this->m_sPath = path;
    } else {
        this->m_sPath = path.parent_path();
    }
    this->m_sFileName = path.stem();

    switch (hash(path.extension().string().c_str())) {
        case ".gmd"_h:   this->m_eFormat = kGmdTypeGmd; break;
        case ".gmd2"_h:  this->m_eFormat = kGmdTypeGmd2; break;
        case ".lvl"_h:   this->m_eFormat = kGmdTypeLvl; break;
    }
}

GmdFile::GmdFile(std::wstring const& pathStr) {
    auto path = fs::path(pathStr);
    if (fs::is_directory(path)) {
        this->m_sPath = path;
    } else {
        this->m_sPath = path.parent_path();
    }
    this->m_sFileName = path.stem();

    switch (hash(path.extension().wstring().c_str())) {
        case L".gmd"_h:  this->m_eFormat = kGmdTypeGmd; break;
        case L".gmd2"_h: this->m_eFormat = kGmdTypeGmd2; break;
        case L".lvl"_h:  this->m_eFormat = kGmdTypeLvl; break;
    }
}

void GmdFile::setFileName(std::string const& name) {
    this->m_sFileName = string_convert(name);
}

void GmdFile::setFileName(std::wstring const& name) {
    this->m_sFileName = name;
}

void GmdFile::setExportFlags(int flags) {
    this->m_nFlags = flags;
}

void GmdFile::setExportFlag(ExportFlag flag) {
    this->m_nFlags |= flag;
}

void GmdFile::setType(GmdType type) {
    this->m_eFormat = type;
}

void GmdFile::removeNullbytesFromString(std::string & str) {
    for (auto i = 0u; i < str.size(); i++) {
        if (!str[i]) str[i] = 32;
    }
}

Result<GJGameLevel*> GmdFile::createLevel(std::string const& data) {
    auto ret = GJGameLevel::create();

    ret->m_eLevelType = kGJLevelTypeEditor;
    ret->m_bIsEditable = true;

    this->removeNullbytesFromString(const_cast<std::string&>(data));

    // DS_Dictionary crashes on large files bruh
    // so i guess we gotta do this manually after all
    tinyxml2::XMLDocument doc;

    auto res = doc.Parse(data.c_str());

    if (res == tinyxml2::XML_NO_ERROR) {
        tinyxml2::XMLNode* first = &doc;
        if (doc.FirstChildElement("d")) {
            first = doc.FirstChildElement("d");
        }

        for (
            auto child = first->FirstChildElement("k");
            child;
            child = child->NextSiblingElement("k")
        ) {
            switch (hash(child->GetText())) {
                case "k2"_h: {
                    auto val = child->NextSiblingElement()->GetText();
                    if (val && strlen(val)) {
                        ret->m_sLevelName = val;
                    }
                } break;

                case "k3"_h: {
                    auto val = child->NextSiblingElement()->GetText();
                    if (val && strlen(val)) {
                        auto dval = decoder::Base64(val);
                        ret->m_sLevelDesc = dval;
                    }
                } break;

                case "k4"_h: {
                    auto val = child->NextSiblingElement()->GetText();
                    if (val && strlen(val)) {
                        ret->setLevelData(val);
                    }
                } break;

                case "k5"_h: {
                    auto val = child->NextSiblingElement()->GetText();
                    if (val && strlen(val)) {
                        ret->m_sCreatorName = val;
                    }
                } break;

                case "k8"_h: {
                    auto val = child->NextSiblingElement()->GetText();
                    if (val && strlen(val)) {
                        try {
                            ret->m_nAudioTrack = std::stoi(val);
                        } catch(...) {}
                    }
                } break;

                case "k45"_h: {
                    auto val = child->NextSiblingElement()->GetText();
                    if (val && strlen(val)) {
                        try {
                            ret->m_nSongID = std::stoi(val);
                        } catch(...) {}
                    }
                } break;
            }
        }
    } else {
        ret->release();
        return Err<>(
            ("XML parse error: "_s + std::to_string(res)).c_str()
        );
    }

    return Ok<>(ret);
}

Result<GJGameLevel*> GmdFile::parseLevel() {
    auto path = this->m_sPath / this->m_sFileName;

    if (!fs::exists(path)) {
        return Err<>(
            "File does not exist! (Likely reason is that the path or "
            "filename contains <co>unrecognized</c> characters; <cy>Move</c> "
            "the file to a different location and try again)"
        );
    }

    switch (this->m_eFormat) {
        case kGmdTypeGmd2: {
            Unzipper zip (path.string());

            std::vector<uint8_t> metaBuffer;
            std::vector<uint8_t> dataBuffer;

            zip.extractEntryToMemory("level.meta", metaBuffer);
            zip.extractEntryToMemory("level.data", dataBuffer);

            if (!metaBuffer.size())
                return Err<>("Unable to read level metadata!");

            if (!dataBuffer.size())
                return Err<>("Unable to read level data!");
            
            std::string metadata = convert_vs(metaBuffer);
            std::string data = convert_vs(dataBuffer);

            nlohmann::json metaj;
            try {
                metaj = nlohmann::json::parse(metadata);
            } catch (...) {
                zip.close();
                return Err<>("Unable to parse metadata!");
            }

            try {
                std::string songfile = metaj["song-file"];

                if (songfile.size()) {
                    if (this->m_nFlags & kfExportFlag_IncludeSong) {
                        if (!zip.extractEntryToMemory(songfile, this->m_vSongData))
                            goto skip_song_file;

                        std::string targetPath;
                        if (metaj["song-is-custom"]) {
                            try {
                                targetPath = MusicDownloadManager::pathForSong(
                                    std::stoi(songfile.substr(0, songfile.find_first_of(".")))
                                );
                            } catch (...) {
                                goto skip_song_file;
                            }
                        } else {
                            targetPath = "Resources/" + songfile;
                        }

                        this->m_sSongPath = string_convert(targetPath);
                    }
                }
            } catch (...) {}
        skip_song_file:

            zip.close();
            
            std::string compr = metaj["compression"];

            compr = sanitizeString(compr);

            data = decodeCompression(data, compr);

            if (!data.size())
                return Err<>("Unable to decode compression!");

            return this->createLevel(data);
        } break;

        case kGmdTypeGmd: {
            auto res = file_read_string(path);

            if (!res || !res.value().size())
                return Err<>("File is empty");

            return this->createLevel(res.value());
        } break;

        case kGmdTypeLvl: {
            auto res = file_read_string(path);

            if (!res || !res.value().length())
                return Err<>("File is empty");
            
            auto data = bgd::gmd::decoder::GZip(res.value());
            data = "<d>" + data + "</d>";

            return this->createLevel(data);
        } break;

        default:
            return Err<>("Invalid Format");
    }

    return Err<>("Unknown Error");
}

Result<std::string> GmdFile::createString() {
    if (!this->m_pLevel) {
        return Err<>("Level is nullptr... somehow");
    }
    
    auto dict = new DS_Dictionary;
    this->m_pLevel->encodeWithCoder(dict);
    std::stringstream res;
    dict->doc.save(res);
    delete dict;

    return Ok<>(res.str());
}

Result<> GmdFile::saveFile() {
    auto str = this->createString();

    auto path = this->m_sPath / this->m_sFileName;
    path.replace_extension(GmdTypeToString(this->m_eFormat));

    if (str) {
        switch (this->m_eFormat) {
            case kGmdTypeGmd2: {
                nlohmann::json metajson = nlohmann::json::object({
                    { "compression", "none" }
                });

                if (this->m_sSongPath.size()) {
                    metajson["song-file"] = fs::path(this->m_sSongPath).filename().string();
                    metajson["song-is-custom"] = this->m_pLevel->m_nSongID;
                }

                std::string metadata = metajson.dump();

                if (fs::exists(path)) {
                    if (!fs::remove(path))
                        return Result<>::err("Unable to write file!");
                }

                Zipper zip (path.string());

                std::istringstream dataStream (str.value());
                std::istringstream metaStream (metadata);

                zip.add(dataStream, "level.data");
                zip.add(metaStream, "level.meta");

                if (this->m_sSongPath.size()) {
                    zip.add(string_convert(this->m_sSongPath));
                }

                zip.close();

                return Ok<>();
            } break;
            
            case kGmdTypeGmd: {
                return file_write_string(path, str.value());
            } break;

            case kGmdTypeLvl: {
                std::string data = str.value().substr(3, str.value().length() - 7);

                return file_write_binary(
                    path,
                    gdcrypto::zlib::deflateBuffer(
                        convert_vs(data)
                    )
                );
            } break;
        }
    } else {
        return str;
    }

    return Ok<>();
}
