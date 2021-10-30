#pragma once

#include "Keybind.hpp"

namespace bgd {
    struct BGD_DLL keybind_category_id {
        const char* value;
        keybind_category_id();
        keybind_category_id(std::string const&);
        keybind_category_id(const char*);
        ~keybind_category_id();
        const char* c_str() const;
        bool operator==(keybind_category_id const&) const;
        operator bool() const;
        keybind_category_id operator=(std::string const&);
    };

    struct BGD_DLL KeybindAction {
        keybind_id id                   = "";
        keybind_category_id category    = "";
        std::string name;
        std::string subcategory         = "";
        std::string description         = "";
        KeybindList defaults;

        bool operator==(KeybindAction const&) const;

        virtual ~KeybindAction();
    };

    struct BGD_DLL KeybindModifier : public KeybindAction {
        virtual ~KeybindModifier();
    };

    struct BGD_DLL TriggerableAction : public KeybindAction {
        std::function<bool(cocos2d::CCNode*, bool)> action = nullptr;
        std::function<bool(cocos2d::CCNode*, keybind_category_id const&, bool)> actionWithID = nullptr;

        virtual void invoke(cocos2d::CCNode* node, bool down) const;
        virtual void invoke(
            cocos2d::CCNode* node,
            keybind_category_id const& id,
            bool down
        ) const;

        virtual ~TriggerableAction();
    };

    struct BGD_DLL RepeatableAction : public TriggerableAction {
        bool repeatChanged  = false;
        bool repeat         = false;
        int  repeatInterval = 100  ;
        int  repeatStart    = 300  ;

        virtual ~RepeatableAction();
    };

    using KeybindActionList = std::vector<KeybindAction*>;
}
