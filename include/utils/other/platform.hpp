#pragma once

#include "../../bgd/BGDMacros.hpp"
#include <string>
#include <vector>
#include <functional>
#include "win32.hpp"

namespace bgd {
    enum FileFlags {
        kfFileDrop      = 0b01,
        kfFileOpenFile  = 0b10,
    };

    void registerFileExtension(
        std::string const& ext,
        FileFlags flags,
        std::function<void(std::vector<std::string> const&)> handler
    );

    void copyToClipboard(std::string const&);
    void copyToClipboard(void*);
    std::string readClipboard();
    void* readClipboardData();
}
