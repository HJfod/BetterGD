#pragma once

#include "Keybind.hpp"
#include "KeybindAction.hpp"

namespace bgd {
    enum KeybindReset {
        kfKeybindResetAll                   = 0b1,
        kfKeybindResetWithDefaults          = 0b10,
        kfKeybindResetWithoutDefaults       = 0b100,
        kfKeybindResetModifierSettings      = 0b1000,
    };
    using KeybindResetFlags = int;

    class BGD_DLL KeybindManager : public BGDSaveManager {
        protected:
            std::unordered_map<keybind_id, KeybindAction*> m_mActions;
            std::unordered_map<Keybind,    std::vector<keybind_id>> m_mKeybinds;
            std::unordered_map<keybind_id, KeybindList> m_mLoadedBinds;
            static constexpr int s_nVersion = 3;

            bool init();
            
            KeybindManager();
            virtual ~KeybindManager();
        
        public:
            static KeybindManager* get();

            void saveJSON(nlohmann::json&) override;
            void loadJSON(nlohmann::json&) override;

            void loadDefaultKeybinds();
            bool addKeybindAction(
                KeybindAction const& action,
                KeybindList   const& defaults,
                keybind_id    const& insertAfter = nullptr
            );

            decltype(m_mKeybinds) getAllKeybinds();
            KeybindActionList getAllActions(keybind_category_id const& categoryFilter = nullptr);
            KeybindActionList getAllActionsForKeybind(keybind_category_id const& category, Keybind const& bind);

            void addKeybind(    keybind_id const& action, Keybind const& bind);
            void removeKeybind( keybind_id const& action, Keybind const& bind);
            void replaceKeybind(keybind_id const& action, Keybind const& bind, Keybind const& replBind);
            void clearKeybinds( keybind_id const& action);
            void clearActions(  Keybind    const& bind);
            void resetToDefault(
                keybind_id const& action,
                KeybindResetFlags resetFlags = kfKeybindResetAll
            );
            void resetAllToDefault(
                KeybindResetFlags resetFlags =
                    kfKeybindResetAll |
                    kfKeybindResetModifierSettings
            );
    
            void handleKeyEvent(
                keybind_category_id const& category,
                Keybind             const& pressed,
                cocos2d::CCNode*           context,
                bool                       down
            );
            void invokeAction(keybind_id const& id, cocos2d::CCNode* context, bool down = true);
            void invokeAction(keybind_id const& id, keybind_category_id const& category, cocos2d::CCNode* context, bool down = true);
            bool isModifierPressed(keybind_id const& id);

            static constexpr int getVersion() { return s_nVersion; }
    };
}
