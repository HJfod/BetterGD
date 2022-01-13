#pragma once

#include <BGDUtils.hpp>
#include <managers/SuperMouseManager.hpp>

namespace bgd {
    class BGD_DLL Scrollbar : public cocos2d::CCLayer, public SuperMouseDelegate {
        protected:
            BoomListView* m_pList = nullptr;
            cocos2d::extension::CCScale9Sprite* m_pBG;
            cocos2d::extension::CCScale9Sprite* m_pTrack;
            cocos2d::CCPoint m_obClickOffset;
            float m_fWidth;

            bool mouseDownSuper(MouseButton, cocos2d::CCPoint const&) override;
            bool mouseUpSuper(MouseButton, cocos2d::CCPoint const&) override;
            void mouseMoveSuper(cocos2d::CCPoint const&) override;
            bool mouseScrollSuper(float y, float x) override;
        
            void draw() override;

            bool init(BoomListView*);

        public:
            void setList(BoomListView* list);

            static Scrollbar* create(BoomListView* list);
    };
}
