#include <BetterGD.hpp>

USE_BGD_NAMESPACE();
using namespace matdash;

                                           // i am incredibly mature
static constexpr const int SCROLLBAR_TAG = 0xB00B1E5;

class CoolScrollbarsPlugin : public BGDPlugin {
    protected:
        void setup() override {
            this->m_sID = "hjfod.cool_scrollbars";
            this->m_sName = "Cool Scrollbars";
            this->m_sDeveloper = "HJfod";
            this->m_sCredits = "Various different chords in Onnanoko ni Naritai";
        }
        BGD_PLUGIN_GET(CoolScrollbarsPlugin);
};

bool LevelBrowserLayer_init(LevelBrowserLayer* self, GJSearchObject* sobj) {
    if (!orig<&LevelBrowserLayer_init>(self, sobj))
        return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bar = Scrollbar::create(self->m_pList->m_pListView);
    bar->setTag(SCROLLBAR_TAG);
    bar->setPosition(winSize.width / 2 + 210.f, winSize.height / 2);
    self->addChild(bar, 256);
    
    return true;
}
static CreateHook<&LevelBrowserLayer_init>$lbli(base + 0x15a040, CoolScrollbarsPlugin::get());

void LevelBrowserLayer_setupLevelBrowser(LevelBrowserLayer* self, CCArray* levels) {
    orig<&LevelBrowserLayer_setupLevelBrowser>(self, levels);
    
    auto bar = as<Scrollbar*>(self->getChildByTag(SCROLLBAR_TAG));
    if (bar) {
        bar->setList(self->m_pList->m_pListView);
    }
}
static CreateHook<&LevelBrowserLayer_setupLevelBrowser>$lblslb(base + 0x15bb40, CoolScrollbarsPlugin::get());

class GJCommentListLayer : public cocos2d::CCLayerColor {
    public:
        BoomListView* m_pList; // 0x198
};

class InfoLayer :
    public FLAlertLayer,
    public LevelCommentDelegate,
    public CommentUploadDelegate,
    public FLAlertLayerProtocol /* 0x1d8 */
{
    public:
        PAD(44)
        GJCommentListLayer* m_pList; // 0x204
};

bool InfoLayer_init(InfoLayer* self, GJGameLevel* level, GJUserScore* score) {
    self->m_pList = nullptr;

    if (!orig<&InfoLayer_init>(self, level, score))
        return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bar = Scrollbar::create(self->m_pList->m_pList);
    bar->setTag(SCROLLBAR_TAG);
    auto pos = CCPoint { winSize.width / 2 + 210.f, winSize.height / 2 };
    if (self->m_pList && self->m_pList->m_pList) {
        pos = self->m_pList->convertToWorldSpace({
            self->m_pList->m_pList->getPositionX() +
                self->m_pList->m_pList->m_fWidth + 150.f,
            self->m_pList->m_pList->getPositionY() +
                self->m_pList->m_pList->m_fHeight
        });
    }
    bar->setPosition(pos.x, pos.y);
    self->addChild(bar, 256);
    
    return true;
}
static CreateHook<&InfoLayer_init>$ili(base + 0x14f5a0, CoolScrollbarsPlugin::get());

void InfoLayer_setupCommentsBrowser(InfoLayer* self, CCArray* comments) {
    orig<&InfoLayer_setupCommentsBrowser>(self, comments);

    auto bar = as<Scrollbar*>(self->getChildByTag(SCROLLBAR_TAG));
    if (bar) {
        bar->setList(self->m_pList->m_pList);
        bar->setPosition(
            self->m_pList->convertToWorldSpace({
                self->m_pList->m_pList->getPositionX() +
                    self->m_pList->m_pList->m_fWidth + 150.f,
                self->m_pList->m_pList->getPositionY() +
                    self->m_pList->m_pList->m_fHeight
            })
        );
    }
}
static CreateHook<&InfoLayer_setupCommentsBrowser>$ilscb(base + 0x152270, CoolScrollbarsPlugin::get());

BGD_LOAD(CoolScrollbarsPlugin);
