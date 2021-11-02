#include "bgd_hook.hpp"

void UILayer_keyDown(UILayer* self, enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), true
    );
}
static InternalCreateHook<&UILayer_keyDown>$uilkd(base + 0x25f890);

void UILayer_keyUp(UILayer* self, enumKeyCodes key) {
    KeybindManager::get()->handleKeyEvent(
        KB_PLAY_CATEGORY, Keybind(key), PlayLayer::get(), false
    );
}
static InternalCreateHook<&UILayer_keyUp>$uilku(base + 0x25fa10);


void EditorUI_keyDown(EditorUI* self_, enumKeyCodes key) {
    auto self = cast::offset_cast<EditorUI*>(self_, -0xf8);
    KeybindManager::get()->handleKeyEvent(
        KB_EDITOR_CATEGORY, Keybind(key), self, true
    );
}
static InternalCreateHook<&EditorUI_keyDown>$euikd(base + 0x91a30);

void EditorUI_keyUp(EditorUI* self_, enumKeyCodes key) {
    auto self = cast::offset_cast<EditorUI*>(self_, -0xf8);
    KeybindManager::get()->handleKeyEvent(
        KB_EDITOR_CATEGORY, Keybind(key), self, false
    );
}
static InternalCreateHook<&EditorUI_keyUp>$euiku(base + 0x92180);

