#pragma once

#include <BGDMacros.hpp>
#include <string>
#include <vector>
#include <functional>
#include "win32.hpp"

namespace bgd {
    enum FileFlags {
        kfFileDrop      = 0b01,
        kfFileOpenFile  = 0b10,
    };

    BGD_DLL void registerFileExtension(
        std::string const& ext,
        FileFlags flags,
        std::function<void(std::vector<std::string> const&)> handler
    );

    BGD_DLL bool copyToClipboard(std::string const& data);
    BGD_DLL std::string readClipboard();

    BGD_DLL bool loadConsole();
    BGD_DLL bool unloadConsole();
    BGD_DLL void bufferConsoleInput();
}
