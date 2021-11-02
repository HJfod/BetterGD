#include "bgd_hook.hpp"
#include "../bgd/BGDInternal.hpp"

void LoadingLayer_loadAssets(LoadingLayer* self) {
    matdash::orig<&LoadingLayer_loadAssets>(self);
    BGDInternal::get()->addResourceSearchPaths();
}
static CreateHook<&LoadingLayer_loadAssets>$llla(base + 0x18c8e0);


