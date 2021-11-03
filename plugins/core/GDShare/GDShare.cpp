#include <BetterGD.hpp>

USE_BGD_NAMESPACE();
using namespace bgd::hook;

class GDSharePlugin : public BGDPlugin {
    protected:
        void setup() override {
            this->m_sID = "hjfod.gdshare";
            this->m_sName = "GDShare";
            this->m_sDeveloper = "HJfod";
            this->m_sCredits = "Mercury6779, Simonious & GDColon";
        }
        BGD_PLUGIN_GET(GDSharePlugin);
};

class LevelBrowserLayer_CB : public LevelBrowserLayer {
    public:
        void onImport(CCObject*) {
            
        }
};

bool LevelBrowserLayer_init(LevelBrowserLayer* self, GJSearchObject* sobj) {
    if (!orig<&LevelBrowserLayer_init>(self, sobj))
        return false;
    
    if (sobj->m_nScreenID == kGJSearchTypeMyLevels) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        CCMenu* btnMenu = getChild<CCMenu*>(self, 8);
        CCNode* newBtn = getChild<CCNode*>(btnMenu, 0);

        auto importButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"),
            btnMenu,
            (SEL_MenuHandler)&LevelBrowserLayer_CB::onImport
        );
        importButton->setPosition(newBtn->getPositionX(), newBtn->getPositionY() + 60.0f);
        btnMenu->addChild(importButton);
    }
    
    return true;
}
static CreateHook<&LevelBrowserLayer_init>$lbli(base + 0x15a040, GDSharePlugin::get());

BGD_LOAD(GDSharePlugin);
