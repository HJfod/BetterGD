#include "bgd_hook.hpp"

void AppDelegate_trySaveGame(CCObject* self) {
    BGDLoader::get()->saveData();
    return matdash::orig<&AppDelegate_trySaveGame>(self);
}
static CreateHook<&AppDelegate_trySaveGame>$aptsg(base + 0x3d5e0);
