#pragma once

#include <BGDMacros.hpp>
#include <string>
#include "types.hpp"
#include "result.hpp"
#include <filesystem>

namespace bgd {
    BGD_DLL Result<std::string> file_read_string(std::string const& path);
    BGD_DLL Result<byte_array>  file_read_binary(std::string const& path);

    BGD_DLL Result<>    write_file_string(std::string const& path, std::string const& data);
    BGD_DLL Result<>    write_file_binary(std::string const& path, byte_array  const& data);

    BGD_DLL Result<>    directory_create(std::string const& path);
    BGD_DLL Result<>    directory_create_all(std::string const& path);

    BGD_DLL class FileCrawler {
        protected:
            std::filesystem::path m_sPath = "";

        public:
            FileCrawler(std::string const& path);
            ~FileCrawler();

            Result<std::string> read();
            Result<byte_array>  readb();
            Result<> write(std::string const&);
            Result<> write(byte_array  const&);
            Result<> peek(std::string const& file, bool create = false);
            Result<> enter(std::string const& dir, bool create = false);
            void leave();
            Result<> push(std::string const& dirs, bool create = false);
            void pop();
            std::vector<std::string> list();
            std::string path();
    };
}
