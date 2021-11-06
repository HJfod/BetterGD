#pragma once

#include <BGDUtils.hpp>
#include "MouseButton.hpp"
#include <set>
#include <unordered_set>

namespace bgd {
    BGD_DLL std::string keyToStringFixed(cocos2d::enumKeyCodes code);

    BGD_DLL bool keyIsModifier(cocos2d::enumKeyCodes code);

    struct BGD_DLL Keybind {
        cocos2d::enumKeyCodes key = cocos2d::KEY_None;
        enum Modifiers : int {
            kmNone      = 0,
            kmControl   = 1,
            kmShift     = 2,
            kmAlt       = 4,
            kmCommand   = 8,
        };
        using ModifierFlags = int;
        ModifierFlags modifiers;
        MouseButton mouse = kMouseButtonNone;

        bool operator==(Keybind const&) const;
        bool operator<(Keybind const&) const;

        std::string toString() const;
        void save(DS_Dictionary*) const;

        Keybind();
        Keybind(cocos2d::enumKeyCodes);
        Keybind(cocos2d::enumKeyCodes, Modifiers);
        Keybind(cocos2d::enumKeyCodes, int);
        Keybind(Modifiers);
        Keybind(int);
        Keybind(MouseButton);
        Keybind(MouseButton, int);
        Keybind(MouseButton, Modifiers);
        Keybind(DS_Dictionary*, int version);
    };

    using KeybindList = std::unordered_set<Keybind>;

    struct BGD_DLL keybind_action_id {
        const char* value;

        keybind_action_id();
        keybind_action_id(std::string const&);
        keybind_action_id(const char*);
        ~keybind_action_id();
        const char* c_str() const;
        bool operator==(keybind_action_id const&) const;
        operator int()  const;
        keybind_action_id operator=(std::string const&);
        keybind_action_id operator=(std::nullptr_t const&);
    };

    BGD_DLL std::ostream& operator<<(std::ostream& stream, Keybind const& bind);
    BGD_DLL std::ostream& operator<<(std::ostream& stream, keybind_action_id const& id);
}

namespace std {
    template<>
    struct hash<bgd::Keybind> {
        BGD_DLL std::size_t operator()(bgd::Keybind const&) const;
    };
    template<>
    struct hash<bgd::keybind_action_id> {
        BGD_DLL std::size_t operator()(bgd::keybind_action_id const&) const;
    };
}
