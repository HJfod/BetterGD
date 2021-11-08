#include "DevTools.hpp"

DevTools::DevTools() {
    this->loadColorScheme();
}

DevTools::~DevTools() {}

DevTools* DevTools::get() {
    static auto g_dev = new DevTools;
    return g_dev;
}

class AccessSpecifiersAreForNerds : public CCTransitionScene {
    public:
        CCScene* getIn()  { return this->m_pInScene; }
        CCScene* getOut() { return this->m_pOutScene; }
};

void DevTools::loadColorScheme() {
    this->m_obBGColor       = to4B(cc3x(0xf));
    this->m_obTextColor     = to4B(cc3x(0x0));
    this->m_obHoverColor    = to4B(cc3x(0x0), 85);
}

void DevTools::draw() {
    if (this->m_bVisible) {
        std::cout << "draw\n";
        glColor3b(255, 0, 0);
        glRecti(0, 0, 10, 40);
    }
}

void DevTools::recurseUpdateList(CCNode* parent) {

}

void DevTools::showAnimation(CCScene* scene, bool transition) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto scale = this->m_bVisible ?
        winSize.width / (winSize.width + this->m_fWidth) :
        1.f;
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
        this->showAnimation(t->getOut(), false);
    }
    this->showAnimation(scene, false);
}

void DevTools::willSwitchToScene(CCScene* scene) {
    this->fixSceneScale(scene);
}

void DevTools::show() {
    if (!this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = true;
        this->showAnimation(scene, true);
    }
}

void DevTools::hide() {
    if (this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = false;
        this->showAnimation(scene, true);
    }
}

void DevTools::toggle() {
    if (this->m_bVisible) {
        this->hide();
    } else {
        this->show();
    }
}
