#include "bgd_hook.hpp"

void UILayer_keyDown(UILayer* self, enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), true
    );
}
static CreateHook<&UILayer_keyDown>$uilkd(base + 0x25f890);

void UILayer_keyUp(UILayer* self, enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), false
    );
}
static CreateHook<&UILayer_keyUp>$uilku(base + 0x25fa10);

