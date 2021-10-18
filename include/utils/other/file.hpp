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
}
