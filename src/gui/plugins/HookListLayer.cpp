#include "HookListLayer.hpp"

bool HookListLayer::init(BGDPlugin* plugin) {
    if (!GJDropDownLayer::init("Hooks", 220.f))
        return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto hooks = CCArray::create();
    for (auto const& hook : plugin->getHooks()) {
        hooks->addObject(new HookItem(hook));
    }
    this->m_pListLayer->m_pListView = HookListView::create(hooks, plugin, 356.f, 220.f);
    this->m_pListLayer->addChild(this->m_pListLayer->m_pListView);

    return true;
}

HookListLayer* HookListLayer::create(BGDPlugin* plugin) {
    auto ret = new HookListLayer;
    if (ret && ret->init(plugin)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
