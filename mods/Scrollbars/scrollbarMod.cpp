#include <BetterGD.hpp>

USE_LILAC_NAMESPACE();
USE_BGD_NAMESPACE();

static constexpr const int SCROLLBAR_TAG = 0xB00B1E5; // i am incredibly mature

bool __fastcall LevelBrowserLayer_init(LevelBrowserLayer* self, edx_t, GJSearchObject* sobj) {
    if (!self->init(sobj))
        return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bar = Scrollbar::create(self->m_pList->m_pListView);
    bar->setTag(SCROLLBAR_TAG);
    bar->setPosition(winSize.width / 2 + 210.f, winSize.height / 2);
    self->addChild(bar, 256);
    
    return true;
}

void __fastcall LevelBrowserLayer_setupLevelBrowser(LevelBrowserLayer* self, edx_t, CCArray* levels) {
    self->setupLevelBrowser(levels);
    
    auto bar = as<Scrollbar*>(self->getChildByTag(SCROLLBAR_TAG));
    if (bar) {
        bar->setList(self->m_pList->m_pListView);
    }
}

bool __fastcall InfoLayer_init(InfoLayer* self, edx_t, GJGameLevel* level, GJUserScore* score) {
    self->m_pList = nullptr;

    if (!self->init(level, score))
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

void __fastcall InfoLayer_setupCommentsBrowser(InfoLayer* self, edx_t, CCArray* comments) {
    self->setupCommentsBrowser(comments);

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

LILAC_API bool LILAC_CALL lilac_load(Mod* mod) {
    mod->addHook(gd_base + 0x15a040, &LevelBrowserLayer_init);
    mod->addHook(gd_base + 0x15bb40, &LevelBrowserLayer_setupLevelBrowser);
    mod->addHook(gd_base + 0x14f5a0, &InfoLayer_init);
    mod->addHook(gd_base + 0x152270, &InfoLayer_setupCommentsBrowser);

    return true;
}

LILAC_API void LILAC_CALL lilac_unload() {
}
