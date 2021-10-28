#pragma once

#include <BGDUtils.hpp>

namespace bgd {
    namespace gmd {
        enum GmdType {
            kGmdTypeGmd,
            kGmdTypeGmd2,
            kGmdTypeLvl,
        };

        constexpr const char*    GmdTypeToString (GmdType);
        constexpr const wchar_t* GmdTypeToWString(GmdType);

        bool isLevelFileName(std::string  const& fname);
        bool isLevelFileName(std::wstring const& fname);
        
        const enum ExportFlag {
            kfExportFlag_None           = 0b0000000,
            kfExportFlag_IncludeSong    = 0b0000001,
        };

        byte_array  convert_vs(std::string const& string);
        std::string convert_vs(byte_array  const& vector);

        namespace encoder {
            BGD_DLL byte_array XOR(   const byte_array & data, int key);
            BGD_DLL byte_array Base64(const byte_array & data);
            BGD_DLL byte_array GZip(  const byte_array & data);
        }

        namespace decoder {
            BGD_DLL std::string XOR(   const std::string & data, int key);
            BGD_DLL std::string Base64(const std::string & data);
            BGD_DLL std::string GZip(  const std::string & data);

            BGD_DLL byte_array XORX(   const byte_array & data, int key);
            BGD_DLL byte_array Base64X(const byte_array & data);
            BGD_DLL byte_array GZipX(  const byte_array & data);
        };

        class BGD_DLL GmdFile {
            protected:
                gd::GJGameLevel* m_pLevel = nullptr;
                std::filesystem::path m_sPath;
                std::wstring m_sFileName;
                int m_nFlags = kfExportFlag_None;
                GmdType m_eFormat = kGmdTypeGmd;
                std::wstring m_sSongPath;
                byte_array m_vSongData;

                void saveFileFormat();

                Result<gd::GJGameLevel*> createLevel(std::string const&);

            public:
                GmdFile(gd::GJGameLevel*);
                GmdFile(std::string  const& path);
                GmdFile(std::wstring const& path);

                Result<gd::GJGameLevel*> parseLevel();
                Result<std::string> createString();
                Result<> saveFile();

                void setFileName(std::string const&);
                void setFileName(std::wstring const&);
                void setType(GmdType);
                void setExportFlags(int);
                void setExportFlag(ExportFlag);

                static void removeNullbytesFromString(std::string &);
        };
    }
}
