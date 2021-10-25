#pragma once

#include "gmd.hpp"

namespace bgd {
    namespace gmd {
        BGD_DLL std::string sanitizeString(std::string const& str, bool actuallyDoIt = true);
        BGD_DLL std::string decodeCompression(std::string const& str, std::string const& compArgs);
        BGD_DLL std::string applyCompression(std::string const& str, std::string const& compArgs);
        BGD_DLL std::string fixPlist(std::string const& str);
    }
}
