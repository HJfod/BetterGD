#include "bgd_hook.hpp"

void AppDelegate_trySaveGame(CCObject* self) {
    BGDLoader::get()->saveData();
    return bgd::hook::orig<&AppDelegate_trySaveGame>(self);
}
static InternalCreateHook<&AppDelegate_trySaveGame>$aptsg(base + 0x3d5e0);
