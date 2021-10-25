#include "bgd_hook.hpp"
#include "../gui/PluginLayer.hpp"

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
static CreateHook<&MenuLayer_init>$mli(base + 0x1907b0);

void MenuLayer_onMoreGames(MenuLayer* self, CCObject* pSender) {
    auto layer = PluginLayer::create();
    cocos2d::CCDirector::sharedDirector()
        ->getRunningScene()
        ->addChild(layer);
    layer->showLayer(false);
}
static CreateHook<&MenuLayer_onMoreGames>$mlomg(base + 0x1919c0);

