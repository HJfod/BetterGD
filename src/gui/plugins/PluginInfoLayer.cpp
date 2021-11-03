#include "PluginInfoLayer.hpp"
#include "HookListLayer.hpp"

void PluginInfoLayer::setup() {
    this->m_bNoElasticity = true;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto nameLabel = CCLabelBMFont::create(
        this->m_pPlugin->getName(), "bigFont.fnt"
    );
    nameLabel->setPosition(winSize.width / 2, winSize.height / 2 + 60.f);
    nameLabel->setScale(.7f);
    this->m_pLayer->addChild(nameLabel);

    auto creatorLabel = CCLabelBMFont::create(
        cstrfmt("by %s", this->m_pPlugin->getDeveloper()), "goldFont.fnt"
    );
    creatorLabel->setPosition(winSize.width / 2, winSize.height / 2 + 30.f);
    creatorLabel->setScale(.6f);
    this->m_pLayer->addChild(creatorLabel);

    auto hooksSpr = ButtonSprite::create(
        "Hooks", 0, 0, "bigFont.fnt", "GJ_button_05.png", 0, .8f
    );
    hooksSpr->setScale(.6f);

    auto hooksBtn = CCMenuItemSpriteExtra::create(
        hooksSpr, this, menu_selector(PluginInfoLayer::onHooks)
    );
    hooksBtn->setPosition(
        30.f, this->m_pLrSize.height / 2 - 25.f
    );
    this->m_pButtonMenu->addChild(hooksBtn);
}

void PluginInfoLayer::onHooks(CCObject*) {
    auto layer = HookListLayer::create(this->m_pPlugin);
    this->addChild(layer);
    layer->showLayer(false);
}

PluginInfoLayer* PluginInfoLayer::create(BGDPlugin* plugin) {
    auto ret = new PluginInfoLayer;
    if (
        ret &&
        (ret->m_pPlugin = plugin) &&
        ret->init(420.f, 280.f)
    ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
