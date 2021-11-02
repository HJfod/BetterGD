#include "bgd_hook.hpp"
#include "../bgd/BGDInternal.hpp"

void GameManager_reloadAllStep2(GameManager* self) {
    bgd::hook::orig<&GameManager_reloadAllStep2>(self);
    BGDInternal::get()->addResourceSearchPaths();
}
static InternalCreateHook<&GameManager_reloadAllStep2>$gmras2(base + 0x21bcf4);

void LoadingLayer_loadAssets(LoadingLayer* self) {
    bgd::hook::orig<&LoadingLayer_loadAssets>(self);
    BGDInternal::get()->addResourceSearchPaths();
}
static InternalCreateHook<&LoadingLayer_loadAssets>$llla(base + 0x18c8e0);


