#pragma once

#include <BGDMacros.hpp>
#include <string>
#include "types.hpp"
#include "result.hpp"

namespace bgd {
    BGD_DLL Result<std::string> file_read_string(std::string const& path);
    BGD_DLL Result<byte_array>  file_read_binary(std::string const& path);

    BGD_DLL Result<>    write_file_string(std::string const& path, std::string const& data);
    BGD_DLL Result<>    write_file_binary(std::string const& path, byte_array  const& data);
}
