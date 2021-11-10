#include "PluginListView.hpp"
#include "PluginInfoLayer.hpp"

PluginCell::PluginCell(const char* name, CCSize size) :
    TableViewCell(name, size.width, size.height) {}

void PluginCell::draw() {
    reinterpret_cast<void(__thiscall*)(PluginCell*)>(
        base + 0x59d40
    )(this);
}

void PluginCell::loadFromPlugin(PluginObject* plugin) {
    this->m_pPlugin = plugin->m_pPlugin;

    this->m_pLayer->setVisible(true);
    this->m_pBGLayer->setOpacity(255);
    
    auto menu = CCMenu::create();
    menu->setPosition(this->m_fWidth - this->m_fHeight, this->m_fHeight / 2);
    this->m_pLayer->addChild(menu);

    auto titleLabel = CCLabelBMFont::create(
        std::string(this->m_pPlugin->getName()).c_str(), "bigFont.fnt"
    );
    titleLabel->setAnchorPoint({ .0f, .5f });
    titleLabel->setScale(.5f);
    titleLabel->setPosition(this->m_fHeight / 2, this->m_fHeight / 2 + 7.f);
    this->m_pLayer->addChild(titleLabel);
    
    auto creatorLabel = CCLabelBMFont::create(
        cstrfmt("by %s", this->m_pPlugin->getDeveloper()), "goldFont.fnt"
    );
    creatorLabel->setAnchorPoint({ .0f, .5f });
    creatorLabel->setScale(.43f);
    creatorLabel->setPosition(this->m_fHeight / 2, this->m_fHeight / 2 - 7.f);
    this->m_pLayer->addChild(creatorLabel);

    auto viewSpr = 
        ButtonSprite::create("View", 0, 0, "bigFont.fnt", "GJ_button_01.png", 0, .8f);
    viewSpr->setScale(.65f);

    auto viewBtn = CCMenuItemSpriteExtra::create(
        viewSpr, this, menu_selector(PluginCell::onInfo)
    );
    menu->addChild(viewBtn);
}

void PluginCell::onInfo(CCObject*) {
    PluginInfoLayer::create(this->m_pPlugin)->show();
}

PluginCell* PluginCell::create(const char* key, CCSize size) {
    auto pRet = new PluginCell(key, size);
    if (pRet) {
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}


void PluginListView::setupList() {
    this->m_fItemSeparation = 40.0f;

    if (!this->m_pEntries->count()) return;

    this->m_pTableView->reloadData();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
    
    this->m_pTableView->moveToTop();
}

TableViewCell* PluginListView::getListCell(const char* key) {
    return PluginCell::create(key, { this->m_fWidth, this->m_fItemSeparation });
}

void PluginListView::loadCell(TableViewCell* cell, unsigned int index) {
    as<PluginCell*>(cell)->loadFromPlugin(
        as<PluginObject*>(this->m_pEntries->objectAtIndex(index))
    );
    as<StatsCell*>(cell)->updateBGColor(index);
}

PluginListView* PluginListView::create(
    CCArray* actions,
    float width, float height
) {
    auto pRet = new PluginListView;
    if (pRet) {
        if (pRet->init(actions, kBoomListType_Plugin, width, height)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
