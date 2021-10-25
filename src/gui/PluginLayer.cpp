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

    return true;
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
