#include "PluginLayer.hpp"

bool PluginLayer::init() {
    if (!GJDropDownLayer::init("Plugins", 220.f))
        return false;
    
    auto pluginArray = vectorToCCArray<BGDPlugin*, PluginObject*>(
        BGDLoader::get()->getAllPlugins(), [](BGDPlugin* p) -> PluginObject* {
            return new PluginObject(p);
        }
    );
    this->m_pListLayer->m_pListView = PluginListView::create(pluginArray, 356.f, 220.f);
    this->m_pListLayer->addChild(this->m_pListLayer->m_pListView);

    auto errorInfoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(PluginLayer::onViewErrors)
    );
    errorInfoButton->setPosition(0, -this->getContentSize().height + 64);
    this->m_pButtonMenu->addChild(errorInfoButton);

    return true;
}

void PluginLayer::onViewErrors(CCObject*) {
    auto errors = BGDLoader::get()->getErrors();
    std::string err = "";
    for (auto const& error : errors) {
        err += error->info;
        err += "; ";
        err += error->description;
        err += "\n ";
    }
    FLAlertLayer::create(
        nullptr, "Error List",
        "OK", nullptr,
        err
    )->show();
}

PluginLayer* PluginLayer::create() {
    auto ret = new PluginLayer;
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
