#include "bgd_hook.hpp"
#include "../gui/keybinds/CustomKeybindingsLayer.hpp"

void MoreOptionsLayer_onKeybindings(MoreOptionsLayer*, CCObject*) {
    CustomKeybindingsLayer::create()->show();
}
static InternalCreateHook<&MoreOptionsLayer_onKeybindings>$molokb(base + 0x749d0);
