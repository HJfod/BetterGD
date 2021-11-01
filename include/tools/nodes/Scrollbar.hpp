#pragma once

#include <BGDUtils.hpp>
#include <tools/managers/SuperMouseManager.hpp>

namespace bgd {
    class BGD_DLL Scrollbar : public cocos2d::CCLayer, public SuperMouseDelegate {
        protected:
            gd::BoomListView* m_pList = nullptr;
            cocos2d::extension::CCScale9Sprite* m_pBG;
            cocos2d::extension::CCScale9Sprite* m_pTrack;
            cocos2d::CCPoint m_obClickOffset;
            float m_fWidth;

            bool mouseDownSuper(MouseButton, cocos2d::CCPoint const&) override;
            bool mouseUpSuper(MouseButton, cocos2d::CCPoint const&) override;
            void mouseMoveSuper(cocos2d::CCPoint const&) override;
            bool mouseScrollSuper(float y, float x) override;
        
            void draw() override;

            bool init(gd::BoomListView*);

        public:
            void setList(gd::BoomListView* list);

            static Scrollbar* create(gd::BoomListView* list);
    };
}
