#include <SuperKeyboardManager.hpp>
#include <algorithm>

USE_BGD_NAMESPACE();

bool SuperKeyboardDelegate::keyDownSuper(enumKeyCodes) { return false; }
bool SuperKeyboardDelegate::keyUpSuper(enumKeyCodes) { return false; }
void SuperKeyboardDelegate::superKeyPushSelf() {
    SuperKeyboardManager::get()->pushDelegate(this);
}
void SuperKeyboardDelegate::superKeyPopSelf() {
    SuperKeyboardManager::get()->popDelegate(this);
}

SuperKeyboardDelegate::SuperKeyboardDelegate() {
    SuperKeyboardManager::get()->pushDelegate(this);
}

SuperKeyboardDelegate::~SuperKeyboardDelegate() {
    SuperKeyboardManager::get()->popDelegate(this);
}

bool SuperKeyboardManager::init() {
    return true;
}

SuperKeyboardManager::SuperKeyboardManager() {
    this->init();
}

SuperKeyboardManager::~SuperKeyboardManager() {
    for (auto const& delegate : this->m_vDelegates) {
        this->popDelegate(delegate);
    }
}

SuperKeyboardManager* SuperKeyboardManager::get() {
    static auto g_manager = new SuperKeyboardManager;
    return g_manager;
}

void SuperKeyboardManager::pushDelegate(SuperKeyboardDelegate* delegate) {
    this->m_vDelegates.insert(this->m_vDelegates.begin(), delegate);
}

void SuperKeyboardManager::popDelegate(SuperKeyboardDelegate* delegate) {
    this->m_vDelegates.erase(std::remove(
        this->m_vDelegates.begin(), this->m_vDelegates.end(), delegate
    ), this->m_vDelegates.end());
}

bool SuperKeyboardManager::dispatchEvent(enumKeyCodes key, bool keydown) {
    if (keydown) {
        this->m_vPressedKeys.insert(key);
    } else {
        this->m_vPressedKeys.erase(key);
    }
    if (this->m_vDelegates.size()) {
        if (keydown) {
            if (this->m_vDelegates.at(0)->keyDownSuper(key))
                return true;
        } else {
            if (this->m_vDelegates.at(0)->keyUpSuper(key))
                return true;
        }
    }
    return false;
}

bool SuperKeyboardManager::isKeyDown(enumKeyCodes key) const {
    return this->m_vPressedKeys.count(key);
}
