#pragma once

#include "Keybind.hpp"

namespace bgd {
    struct KeybindCallback {
        keybind_id id = "";
        std::string name;
        std::string subcategory = "";
        std::string description = "";
        KeybindList defaults;
        bool repeatable = true;
        bool repeatChanged = false;
        bool repeat = false;
        int repeatInterval = 100;
        int repeatStart = 300;
        bool modifier = false;
        std::function<bool(cocos2d::CCNode*, bool)> callback;

        bool operator==(KeybindCallback const&) const;

        virtual inline void z() {}
    };
}
