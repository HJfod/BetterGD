#include "KeybindListView.hpp"

KeybindCell::KeybindCell(const char* name, CCSize size) :
    TableViewCell(name, size.width, size.height) {}

void KeybindCell::loadFromItem(KeybindItem* item) {
    this->m_pItem = item;

    this->m_pBGLayer->setOpacity(255);

    auto name = item->m_sText.c_str();
    if (!item->m_sText.size()) {
        name = item->m_pAction->name.c_str();
    }

    this->m_pMenu = CCMenu::create();
    this->m_pMenu->setPosition(m_fWidth / 2, m_fHeight / 2);
    this->m_pLayer->addChild(this->m_pMenu);

    auto nameLabel = CCLabelBMFont::create(name, "bigFont.fnt");
    nameLabel->limitLabelWidth(140.0f, .5f, .0f);
    nameLabel->setAnchorPoint({ 0.0f, 0.5f });
    if (!item->m_pAction) {
        nameLabel->setOpacity(180);
        nameLabel->setColor({ 180, 180, 180 });
    } else {
        if (dynamic_cast<KeybindModifier*>(item->m_pAction))
            nameLabel->setColor(cc3x(0x8fa));
    }

    auto nameBtn = CCMenuItemSpriteExtra::create(
        nameLabel, this, menu_selector(KeybindCell::onDescription)
    );
    nameBtn->setPosition( -this->m_fWidth / 2 + 15.0f, 0.f );
    nameBtn->setAnchorPoint({ 0.0f, 0.5f });

    auto nameMenu = CCMenu::create();
    nameMenu->addChild(nameBtn);
    nameMenu->setPosition(this->m_fWidth / 2, this->m_fHeight / 2);
    this->m_pLayer->addChild(nameMenu);

    if (!item->m_pAction) {
        nameBtn->setEnabled(false);
    }

    // if (item->m_sText.size()) {
    //     auto foldBtn = CCMenuItemToggler::create(
    //         createKeybindBtnSprite("-", false),
    //         createKeybindBtnSprite("+", false),
    //         this,
    //         menu_selector(KeybindCell::onFold)
    //     );
    //     foldBtn->toggle(m_pItem->delegate->m_mFoldedCategories[m_pItem->m_sText]);
    //     foldBtn->setPosition(m_fWidth / 2 - 15.0f, 0.0f);
    //     this->m_pMenu->addChild(foldBtn);
    // }

    this->updateMenu();
}

void KeybindCell::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (btn2) {
        KeybindManager::get()->resetToDefault(
            this->m_pItem->m_pAction->id
        );
        this->updateMenu();
    }
}

void KeybindCell::updateBGColor(int index) {
    this->m_pBGLayer->setColor({ 0, 0, 0 });
    this->m_pBGLayer->setOpacity(index % 2 ? 120 : 70);
}

void KeybindCell::onDescription(CCObject*) {
    if (this->m_pItem->m_pAction) {
        auto desc = this->m_pItem->m_pAction->description;
        if (!desc.size())
            desc = "No Description Provided";

        FLAlertLayer::create(
            nullptr,
            this->m_pItem->m_pAction->name.c_str(),
            "OK", nullptr,
            360.f, desc
        )->show();
    }
}

void KeybindCell::updateMenu() {}

KeybindCell* KeybindCell::create(const char* key, CCSize size) {
    auto pRet = new KeybindCell(key, size);
    if (pRet) {
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}


void KeybindListView::setupList() {
    this->m_fItemSeparation = 27.5f;

    this->m_pTableView->reloadData();

    this->m_pTableView->m_fScrollLimitTop = this->m_fItemSeparation *
        (this->m_pTableView->m_pContentLayer->getScaledContentSize().height / 1500.0f);

    this->m_pTableView->moveToTop();
}

TableViewCell* KeybindListView::getListCell(const char* key) {
    return KeybindCell::create(key, { this->m_fWidth, this->m_fItemSeparation });
}

void KeybindListView::loadCell(TableViewCell* cell, unsigned int index) {
    as<KeybindCell*>(cell)->loadFromItem(
        as<KeybindItem*>(this->m_pEntries->objectAtIndex(index))
    );
    as<KeybindCell*>(cell)->updateBGColor(index);
}

KeybindListView* KeybindListView::create(CCArray* binds, float width, float height) {
    auto pRet = new KeybindListView;

    if (pRet && pRet->init(binds, kBoomListType_Keybind, width, height)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
