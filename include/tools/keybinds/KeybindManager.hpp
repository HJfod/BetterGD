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
            struct repeat_info {
                float time;
                cocos2d::CCNode* context;
                keybind_category_id category;
            };

            std::unordered_map<keybind_action_id, KeybindAction*> m_mActions;
            std::unordered_map<Keybind,           std::vector<keybind_action_id>> m_mKeybinds;
            std::unordered_map<keybind_action_id, KeybindList> m_mLoadedBinds;
            std::unordered_map<keybind_action_id, repeat_info> m_mRepeat;
            std::unordered_map<keybind_category_id, int>       m_mCategoryInfo;
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
                KeybindAction     const& action,
                KeybindList       const& defaults,
                keybind_action_id const& insertAfter = nullptr
            );
            bool removeKeybindAction(keybind_action_id const& id);

            decltype(m_mKeybinds) getAllKeybinds();
            KeybindActionList getAllActions(keybind_category_id const& categoryFilter = nullptr);
            KeybindActionList getAllActionsForKeybind(keybind_category_id const& category, Keybind const& bind);
            KeybindList getKeybindsForAction(keybind_action_id const& action);

            std::vector<keybind_category_id> getAllCategories() const;
            KeybindActionList getAllActionsInCategory(keybind_category_id const& id) const;
            int getActionCountInCategory(keybind_category_id const& id);

            void addKeybind(    keybind_action_id const& action, Keybind const& bind);
            void removeKeybind( keybind_action_id const& action, Keybind const& bind);
            void replaceKeybind(keybind_action_id const& action, Keybind const& bind, Keybind const& replBind);
            void clearKeybinds( keybind_action_id const& action);
            void clearActions(  Keybind           const& bind);
            void resetToDefault(
                keybind_action_id const& action,
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
            void invokeAction(keybind_action_id const& id, cocos2d::CCNode* context, bool down = true);
            void invokeAction(keybind_action_id const& id, keybind_category_id const& category, cocos2d::CCNode* context, bool down = true);
            void invokeAction(KeybindAction* action, cocos2d::CCNode* context, bool down = true);
            void invokeAction(KeybindAction* action, keybind_category_id const& category, cocos2d::CCNode* context, bool down = true);
            bool isModifierPressed(keybind_action_id const& id);

            RepeatableAction* isRepeatableAction(keybind_action_id const& id);
            void handleRepeats(float deltaTime);

            static constexpr int getVersion() { return s_nVersion; }
    };
}
