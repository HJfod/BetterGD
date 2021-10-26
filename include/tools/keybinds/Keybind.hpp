#pragma once

#include <BGDUtils.hpp>
#include "MouseButton.hpp"
#include <set>

namespace bgd {
    BGD_DLL std::string keyToStringFixed(cocos2d::enumKeyCodes code);

    struct Keybind {
        cocos2d::enumKeyCodes key = cocos2d::KEY_None;
        enum Modifiers : int {
            kmNone      = 0,
            kmControl   = 1,
            kmShift     = 2,
            kmAlt       = 4,
        };
        int modifiers;
        MouseButton mouse = kMouseButtonNone;

        bool operator==(Keybind const&) const;
        bool operator<(Keybind const&) const;

        BGD_DLL std::string toString() const;
        BGD_DLL void save(DS_Dictionary*) const;

        Keybind();
        Keybind(cocos2d::enumKeyCodes);
        Keybind(cocos2d::enumKeyCodes, Modifiers);
        Keybind(cocos2d::enumKeyCodes, int);
        Keybind(MouseButton);
        Keybind(MouseButton, int);
        Keybind(MouseButton, Modifiers);
        BGD_DLL Keybind(DS_Dictionary*, int version);
    };

    using KeybindList = std::set<Keybind>;

    struct keybind_id {
        const char* value;

        keybind_id();
        keybind_id(std::string const&);
        keybind_id(const char*);
        ~keybind_id();
        const char* c_str() const;
        bool operator==(keybind_id const&) const;
        keybind_id operator=(std::string const&);
    };
}

namespace std {
    template<>
    struct hash<bgd::Keybind> {
        BGD_DLL std::size_t operator()(bgd::Keybind const&) const;
    };
    template<>
    struct hash<bgd::keybind_id> {
        BGD_DLL std::size_t operator()(bgd::keybind_id const&) const;
    };
}
