#pragma once

#include <BGDUtils.hpp>

namespace bgd {
    namespace gmd {
        enum GmdType {
            kGmdTypeGmd,
            kGmdTypeGmd2,
            kGmdTypeLvl,
        };

        constexpr const char* GmdTypeToString(GmdType);

        bool isLevelFileName(std::string  const& fname);
        bool isLevelFileName(std::wstring const& fname);
        
        const enum ExportFlag {
            kfExportFlag_None           = 0b0000000,
            kfExportFlag_IncludeSong    = 0b0000001,
        };

        byte_array  convert_vs(std::string const& string);
        std::string convert_vs(byte_array  const& vector);

        namespace encoder {
            BGD_DLL std::vector<uint8_t> XOR(const std::vector<uint8_t> & data, int key);
            BGD_DLL std::vector<uint8_t> Base64(const std::vector<uint8_t> & data);
            BGD_DLL std::vector<uint8_t> GZip(const std::vector<uint8_t> & data);
        }

        namespace decoder {
            BGD_DLL std::string XOR(const std::string & data, int key);
            BGD_DLL std::string Base64(const std::string & data);
            BGD_DLL std::string GZip(const std::string & data);

            BGD_DLL std::vector<uint8_t> XORX(const std::vector<uint8_t> & data, int key);
            BGD_DLL std::vector<uint8_t> Base64X(const std::vector<uint8_t> & data);
            BGD_DLL std::vector<uint8_t> GZipX(const std::vector<uint8_t> & data);
        };

        class BGD_DLL GmdFile {
            protected:
                gd::GJGameLevel* m_pLevel = nullptr;
                std::string m_sPath;
                std::string m_sFullPath;
                std::string m_sFileName;
                int m_nFlags = kfExportFlag_None;
                GmdType m_eFormat = kGmdTypeGmd;
                std::string m_sSongPath;
                byte_array m_vSongData;

                void saveFileFormat();

                Result<gd::GJGameLevel*> createLevel(std::string const&);

            public:
                GmdFile(gd::GJGameLevel*);
                GmdFile(std::string const& path);

                Result<gd::GJGameLevel*> parseLevel();
                Result<std::string> createString();
                Result<> saveFile();

                void setFileName(std::string const&);
                void setType(GmdType);
                void setExportFlags(int);
                void setExportFlag(ExportFlag);

                static void removeNullbytesFromString(std::string &);
        };
    }
}
