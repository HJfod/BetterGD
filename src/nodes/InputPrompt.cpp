#include <InputPrompt.hpp>

USE_BGD_NAMESPACE();

bool InputPrompt::init(const char* title, const char* inputText, InputResult resFunc, const char* applyText) {
    if (!BrownAlertDelegate::init(220.0f, 140.0f, "GJ_square02.png", title))
        return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->m_pInput = InputNode::create(160.0f, inputText, "bigFont.fnt");
    this->m_pInput->setPosition(winSize / 2);
    this->m_pLayer->addChild(this->m_pInput);
    this->m_pResultFunc = resFunc;

    auto button = CCMenuItemSpriteExtra::create(
        ButtonSprite::create(
            applyText, 0, 0, "goldFont.fnt", "GJ_button_01.png", 0, .8f
        ),
        this,
        (SEL_MenuHandler)&InputPrompt::onApply
    );
    button->setPosition(0.0f, - this->m_pLrSize.height / 2 + 25.0f);

    this->m_pButtonMenu->addChild(button);

    this->m_pTrashBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png"),
        this,
        (SEL_MenuHandler)&InputPrompt::onTrash
    );
    this->m_pTrashBtn->setPosition(
        CCPoint { 0, 0 } - this->m_pLrSize / 2 + CCPoint { 25.0f, 25.0f }
    );
    this->m_pTrashBtn->setVisible(false);

    this->m_pButtonMenu->addChild(this->m_pTrashBtn);

    return true;
}

void InputPrompt::onTrash(CCObject*) {
    this->m_pInput->setString("");

    if (!this->m_bApplyOnEsc)
        this->m_pResultFunc("");

    this->onClose(nullptr);
}

void InputPrompt::onApply(CCObject*) {
    this->m_pResultFunc(this->m_pInput->getString());

    this->onClose(nullptr);
}

void InputPrompt::onClose(CCObject*) {
    if (this->m_bApplyOnEsc)
        this->m_pResultFunc(this->m_pInput->getString());

    BrownAlertDelegate::onClose(nullptr);
}

InputPrompt* InputPrompt::create(const char* title, const char* inputText, InputResult resFunc, const char* applyText) {
    auto pRet = new InputPrompt();

    if (pRet && pRet->init(title, inputText, resFunc, applyText)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
