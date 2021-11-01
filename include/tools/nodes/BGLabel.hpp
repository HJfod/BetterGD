#pragma once

#include <BGDUtils.hpp>

namespace bgd {
    class BGD_DLL BGLabel : public cocos2d::CCNode {
        protected:
            cocos2d::extension::CCScale9Sprite* m_pBGSprite;
            cocos2d::CCLabelBMFont* m_pLabel;

            bool init(const char* text, const char* font);
        
        public:
            void setString(const char* text);
            void updateSize();
            void setBGOpacity(GLubyte);

            static BGLabel* create(const char* text, const char* font);
    };
}
