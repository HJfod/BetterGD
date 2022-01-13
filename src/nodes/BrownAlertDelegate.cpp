#include <BrownAlertDelegate.hpp>

USE_BGD_NAMESPACE();

bool BrownAlertDelegate::init(float _w, float _h, const char* _spr, const char* _title) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->m_pLrSize = CCSize { _w, _h };

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    this->m_pLayer = CCLayer::create();
    this->addChild(this->m_pLayer);

    auto bg = CCScale9Sprite::create(_spr, { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(this->m_pLrSize);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    this->m_pLayer->addChild(bg);
    this->m_pBGSprite = bg;

    this->m_pButtonMenu = CCMenu::create();
    this->m_pLayer->addChild(this->m_pButtonMenu);

    if (sizeof _title > 0) {
        auto title = CCLabelBMFont::create(_title, "goldFont.fnt");

        title->limitLabelWidth(this->m_pLrSize.width * 4, .75f, .2f);
        title->setPosition(
            winSize.width / 2,
            winSize.height / 2 + this->m_pLrSize.height / 2 - 25
        );

        this->m_pLayer->addChild(title);
    }

    setup();

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    this->registerWithTouchDispatcher();
    
    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(1.0f);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        (SEL_MenuHandler)&BrownAlertDelegate::onClose
    );
    closeBtn->setUserData(reinterpret_cast<void*>(this));

    this->m_pButtonMenu->addChild(closeBtn);

    closeBtn->setPosition( - _w / 2, _h / 2 );

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

void BrownAlertDelegate::keyDown(enumKeyCodes key) {
    if (key == KEY_Escape)
        return onClose(nullptr);
    if (key == KEY_Space)
        return;
    
    return FLAlertLayer::keyDown(key);
}

void BrownAlertDelegate::onClose(CCObject* pSender) {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
};

