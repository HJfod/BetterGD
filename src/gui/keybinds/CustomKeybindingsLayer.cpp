#include "CustomKeybindingsLayer.hpp"

void CustomKeybindingsLayer::reloadList() {}

void CustomKeybindingsLayer::onKeymap(CCObject*) {}
void CustomKeybindingsLayer::onFinishSelect(CCObject*) {}
void CustomKeybindingsLayer::onGlobalSettings(CCObject*) {}
void CustomKeybindingsLayer::onResetAll(CCObject*) {}

void CustomKeybindingsLayer::setup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->m_bNoElasticity = true;

    auto title = CCLabelBMFont::create("Key Bindings", "bigFont.fnt");
    title->setPosition(winSize.width / 2, winSize.height / 2 + 140.0f - 24.0f);
    title->setScale(.8f);
    this->m_pLayer->addChild(title);
    
    this->m_pSearchBar = InputNode::create(425.0f, "Search Keybinds");
    this->m_pSearchBar->setPosition(title->getPosition() + CCPoint { 0.0f, -35.0f });
    this->m_pSearchBar->getInputNode()->setPositionX(this->m_pSearchBar->getInputNode()->getPositionX() - 200.0f);
    this->m_pSearchBar->setString(this->m_sSearchQuery.c_str());
    CCARRAY_FOREACH_B_TYPE(
        this->m_pSearchBar->getInputNode()->getChildren(), c, CCNode
    ) c->setAnchorPoint({ .0f, .5f });
    this->m_pSearchBar->setScale(.8f);
    this->m_pSearchBar->getInputNode()->setDelegate(this);
    this->m_pLayer->addChild(this->m_pSearchBar);

    this->m_pScrollbar = Scrollbar::create(nullptr);
    this->m_pScrollbar->setPosition(winSize.width / 2 + 190.0f, winSize.height / 2 - 30.0f);
    this->m_pLayer->addChild(this->m_pScrollbar, 800);
    
    this->reloadList();

    {
        auto topItem = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
        topItem->setPosition({
            winSize.width / 2,
            winSize.height / 2 + 55.0f
        });
        topItem->setZOrder(500);
        this->m_pLayer->addChild(topItem);

        auto bottomItem = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
        bottomItem->setPosition({
            winSize.width / 2,
            winSize.height / 2 - 115.0f
        });
        bottomItem->setZOrder(500);
        bottomItem->setFlipY(true);
        this->m_pLayer->addChild(bottomItem);

        auto sideItem = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
        sideItem->setPosition({
            winSize.width / 2 - 173.5f,
            winSize.height / 2 - 29.0f
        });
        sideItem->setZOrder(500);
        sideItem->setScaleY(5.0f);
        this->m_pLayer->addChild(sideItem);

        auto sideItemRight = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
        sideItemRight->setPosition({
            winSize.width / 2 + 173.5f,
            winSize.height / 2 - 29.0f
        });
        sideItemRight->setZOrder(500);
        sideItemRight->setScaleY(5.0f);
        sideItemRight->setFlipX(true);
        this->m_pLayer->addChild(sideItemRight);
    }

    this->m_pSelectLabel = BGLabel::create("", "goldFont.fnt");
    this->m_pSelectLabel->setVisible(false);
    this->m_pSelectLabel->setPosition(winSize.width / 2, winSize.height / 2 + 120.0f);
    this->m_pLayer->addChild(this->m_pSelectLabel);

    auto resetSpr = ButtonSprite::create(
        "Reset", 0, 0, "bigFont.fnt", "GJ_button_05.png", 0, .8f
    );
    resetSpr->setScale(.6f);

    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetSpr,
        this,
        menu_selector(CustomKeybindingsLayer::onResetAll)
    );
    resetBtn->setPosition(210.0f - 40.0f, 140.0f - 25.0f);
    this->m_pButtonMenu->addChild(resetBtn);

    auto mapSpr = ButtonSprite::create(
        "Map", 0, 0, "bigFont.fnt", "GJ_button_05.png", 0, .8f
    );
    mapSpr->setScale(.6f);

    auto mapBtn = CCMenuItemSpriteExtra::create(
        mapSpr,
        this,
        menu_selector(CustomKeybindingsLayer::onKeymap)
    );
    mapBtn->setPosition(210.0f, - 140.0f);
    this->m_pButtonMenu->addChild(mapBtn);

    auto selectSpr = ButtonSprite::create(
        "Cancel", 0, 0, "bigFont.fnt", "GJ_button_06.png", 0, .8f
    );
    selectSpr->setScale(.7f);

    this->m_pSelectBtn = CCMenuItemSpriteExtra::create(
        selectSpr,
        this,
        menu_selector(CustomKeybindingsLayer::onFinishSelect)
    );
    this->m_pSelectBtn->setPosition(0.0f, - 140.0f);
    this->m_pSelectBtn->setVisible(false);
    this->m_pButtonMenu->addChild(this->m_pSelectBtn);

    auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    settingsSpr->setScale(.6f);

    auto settingsBtn = CCMenuItemSpriteExtra::create(
        settingsSpr,
        this,
        menu_selector(CustomKeybindingsLayer::onGlobalSettings)
    );
    settingsBtn->setPosition(- 210.0f + 5.0f, - 140.0f + 5.0f);
    this->m_pButtonMenu->addChild(settingsBtn);

}

CustomKeybindingsLayer* CustomKeybindingsLayer::create() {
    auto ret = new CustomKeybindingsLayer;
    if (ret && ret->init(420.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
