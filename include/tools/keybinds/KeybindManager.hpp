#pragma once

#include "Keybind.hpp"
#include "KeybindAction.hpp"

namespace bgd {
    class BGD_DLL KeybindManager : public BGDSaveManager {
        protected:
            KeybindActionList m_vActions;
            std::unordered_map<Keybind, KeybindAction> m_mKeybinds;
            static constexpr int s_nVersion = 3;

            bool init();
            
            KeybindManager();
            virtual ~KeybindManager();
        
        public:
            static KeybindManager* get();

            void saveJSON(nlohmann::json&) override;
            void loadJSON(nlohmann::json&) override;
    };
}
