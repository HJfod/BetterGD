#include <tools/keybinds/KeybindManager.hpp>

USE_BGD_NAMESPACE();

bool KeybindManager::init() {
    this->m_sSaveDirectory = "";
    this->m_sSaveFileName = "BGDKeybindManager.json";
    this->m_eSaveType = kBGDSaveTypeJSON;

    return true;
}

KeybindManager::KeybindManager() {
    this->init();
}

KeybindManager::~KeybindManager() {}

KeybindManager* KeybindManager::get() {
    static auto g_manager = new KeybindManager;
    return g_manager;
}

void KeybindManager::saveJSON(nlohmann::json&) {

}

void KeybindManager::loadJSON(nlohmann::json&) {
    
}
