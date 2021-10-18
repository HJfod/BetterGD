#pragma once

#include <BGDMacros.hpp>
#include <string>
#include "types.hpp"
#include "result.hpp"
#include <filesystem>

namespace bgd {
    BGD_DLL Result<std::string> file_read_string(std::string const& path);
    BGD_DLL Result<byte_array>  file_read_binary(std::string const& path);

    BGD_DLL Result<>    file_write_string(std::string const& path, std::string const& data);
    BGD_DLL Result<>    file_write_binary(std::string const& path, byte_array  const& data);

    BGD_DLL Result<>    directory_create(std::string const& path);
    BGD_DLL Result<>    directory_create_all(std::string const& path);
    BGD_DLL Result<std::vector<std::string>> directory_list(std::string const& path);

    class BGD_DLL FileCrawler {
        protected:
            std::filesystem::path m_sPath = "";
            bool m_bIsFile = false;

        public:
            FileCrawler(std::string const& path);
            ~FileCrawler();

            Result<std::string> read();
            Result<byte_array>  readb();
            Result<> write(std::string const&);
            Result<> writeb(byte_array  const&);
            Result<> enter(std::string const& path, bool create = false);
            void leave();
            void peek(std::vector<std::string> const& files);
            void visit(std::vector<std::string> const& dirs);
            std::vector<std::string> list();
            std::filesystem::path path();
            std::string paths();
    };
}
