#include "bgd_hook.hpp"
#include "../gui/plugins/PluginLayer.hpp"
#include "../config.h"

bool MenuLayer_init(MenuLayer* self) {
    if (!matdash::orig<&MenuLayer_init>(self))
        return false;

    auto title = as<CCSprite*>(self->getChildren()->objectAtIndex(0));
    auto label = CCLabelBMFont::create("Better", "goldFont.fnt");
    label->setPosition(
        title->getContentSize().height + 20.f,
        title->getContentSize().height + 5.f
    );
    label->setScale(1.f);
    title->addChild(label);
    auto verLabel = CCLabelBMFont::create("v" BGD_VERSION, "goldFont.fnt");
    verLabel->setPosition(
        title->getContentSize().width - title->getContentSize().height,
        5.0f
    );
    verLabel->setScale(.5f);
    title->addChild(verLabel);
    
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

