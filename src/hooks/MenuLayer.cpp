#include "bgd_hook.hpp"

bool MenuLayer_init(MenuLayer* self) {
    if (!matdash::orig<&MenuLayer_init>(self))
        return false;
    
    FLAlertLayer::create(
        nullptr,
        "wow",
        "ok", nullptr,
        "yeah"
    )->show();
    
    return true;
}
CREATE_HOOK(MenuLayer_init, 0x1907b0);

void MenuLayer_onMoreGames(MenuLayer* self, CCObject* pSender) {
    auto np = BGDLoader::get()->updatePlugins();
    FLAlertLayer::create(
        nullptr,
        "wow",
        "ok", nullptr,
        std::to_string(np) + " plugins loaded"
    )->show();
}
CREATE_HOOK(MenuLayer_onMoreGames, 0x1919c0);

