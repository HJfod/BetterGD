#include "PluginInfoLayer.hpp"
#include "HookListLayer.hpp"

void PluginInfoLayer::setup() {
    this->m_bNoElasticity = true;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto nameLabel = CCLabelBMFont::create(
        std::string(this->m_pPlugin->getName()).c_str(), "bigFont.fnt"
    );
    nameLabel->setPosition(winSize.width / 2, winSize.height / 2 + 110.f);
    nameLabel->setScale(.7f);
    this->m_pLayer->addChild(nameLabel, 2); 

    auto creatorLabel = CCLabelBMFont::create(
        cstrfmt("by %s", this->m_pPlugin->getDeveloper()), "goldFont.fnt"
    );
    creatorLabel->setPosition(winSize.width / 2, winSize.height / 2 + 85.f);
    creatorLabel->setScale(.8f);
    this->m_pLayer->addChild(creatorLabel);

    auto descBG = CCScale9Sprite::create("square02b_001.png");
    descBG->setPosition(winSize.width / 2, winSize.height / 2 + 30.f);
    descBG->setContentSize({ 700.f, 120.f });
    descBG->setScale(.5f);
    descBG->setOpacity(100);
    descBG->setColor(cc3x(0));
    this->m_pLayer->addChild(descBG);

    auto desc = this->m_pPlugin->getDescription().size() ?
        std::string(this->m_pPlugin->getDescription()) :
        "[No Description Provided]";

    auto descLabel = TextArea::create(
        "chatFont.fnt", false, desc,
        1.0f, 330.f, 50.f, { .5f, .5f } 
    );
    descLabel->setPosition({ winSize.width / 2, winSize.height / 2 + 30.f });
    this->m_pLayer->addChild(descLabel, 2);

    auto creditsBG = CCScale9Sprite::create("square02b_001.png");
    creditsBG->setPosition(winSize.width / 2, winSize.height / 2 - 50.f);
    creditsBG->setContentSize({ 700.f, 120.f });
    creditsBG->setScale(.5f);
    creditsBG->setOpacity(100);
    creditsBG->setColor(cc3x(0));
    this->m_pLayer->addChild(creditsBG);

    auto credits = this->m_pPlugin->getCredits().size() ?
        "Credits: "_s + std::string(this->m_pPlugin->getCredits()) :
        "[No Credits Provided]";

    auto creditsLabel = TextArea::create(
        "chatFont.fnt", false, credits,
        1.0f, 300.f, 50.f, { .5f, .5f } 
    );
    creditsLabel->setPosition({ winSize.width / 2, winSize.height / 2 - 50.f });
    this->m_pLayer->addChild(creditsLabel, 2);

    auto hooksSpr = ButtonSprite::create(
        "Hooks", 0, 0, "bigFont.fnt", "GJ_button_05.png", 0, .8f
    );
    hooksSpr->setScale(.6f);

    auto hooksBtn = CCMenuItemSpriteExtra::create(
        hooksSpr, this, menu_selector(PluginInfoLayer::onHooks)
    );
    hooksBtn->setPosition(
        -this->m_pLrSize.width / 2 + 45.f,
        -this->m_pLrSize.height / 2 + 25.f
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
