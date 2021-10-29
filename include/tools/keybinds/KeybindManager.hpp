#pragma once

#include "Keybind.hpp"
#include "KeybindAction.hpp"

namespace bgd {
    class BGD_DLL KeybindManager : public BGDSaveManager {
        protected:
            KeybindActionList m_vActions;
            std::unordered_map<Keybind, KeybindAction*> m_mKeybinds;
            static constexpr int s_nVersion = 3;

            bool init();
            
            KeybindManager();
            virtual ~KeybindManager();
        
        public:
            static KeybindManager* get();

            void saveJSON(nlohmann::json&) override;
            void loadJSON(nlohmann::json&) override;

            void loadDefaultKeybinds();
            bool addKeybind(
                KeybindAction const& action,
                KeybindList   const& defaults,
                keybind_id    const& insertAfter = nullptr
            );

            decltype(m_mKeybinds) getAllKeybinds();
            KeybindActionList getAllActions(keybind_category_id const& categoryFilter = nullptr);
            KeybindActionList getAllActionsForKeybind(keybind_category_id const& category, Keybind const& bind);
    
            void handleKeyEvent(
                keybind_category_id const& category,
                Keybind             const& pressed,
                CCNode*                    context,
                bool                       down
            );
            void invokeAction(keybind_id const& id, CCNode* context);
            bool isModifierPressed(keybind_id const& id);

            constexpr int getVersion() const;
    };
}
