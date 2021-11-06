#include "DevTools.hpp"

DevTools* DevTools::create() {
    auto res = new DevTools;
    res->retain();
    return res;
}

DevTools* DevTools::get() {
    static auto g_dev = DevTools::create();
    return g_dev;
}

class AccessSpecifiersAreForNerds : public CCTransitionScene {
    public:
        CCScene* getIn()  { return this->m_pInScene; }
        CCScene* getOut() { return this->m_pOutScene; }
};

void DevTools::setSceneScale(CCScene* scene, bool transition) {
    auto scale = this->m_bVisible ? .7f : 1.f;
    scene->setAnchorPoint({ 1.f, .5f });
    if (transition) {
        scene->runAction(CCEaseInOut::create(
            CCScaleTo::create(.5f, scale), 2.f
        ));
    } else {
        scene->setScale(scale);
    }
}

void DevTools::fixSceneScale(CCScene* scene) {
    auto t = as<AccessSpecifiersAreForNerds*>(
        dynamic_cast<CCTransitionScene*>(scene)
    );
    if (t) {
        scene = t->getIn();
        this->setSceneScale(t->getOut(), false);
    }
    this->setSceneScale(scene, false);
}

void DevTools::willSwitchToScene(CCScene* scene) {
    this->fixSceneScale(scene);
    this->removeFromParent();
    scene->addChild(this);
}

void DevTools::show() {
    if (!this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = true;
        this->setSceneScale(scene, true);
    }
}

void DevTools::hide() {
    if (this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = false;
        this->setSceneScale(scene, true);
    }
}

void DevTools::toggle() {
    if (this->m_bVisible) {
        this->hide();
    } else {
        this->show();
    }
}
