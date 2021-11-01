#pragma once

#include <BGDUtils.hpp>

namespace bgd {
    class BGD_DLL BrownAlertDelegate : public gd::FLAlertLayer {
        protected:
            // layer size is stored here
            cocos2d::CCSize m_pLrSize;
            cocos2d::extension::CCScale9Sprite* m_pBGSprite;

            virtual bool init(
                float width,
                float height,
                const char* bg = "GJ_square01.png",
                const char* title = ""
            );
            virtual void setup() = 0;
            virtual void keyDown(cocos2d::enumKeyCodes) override;

            virtual void onClose(cocos2d::CCObject*);
    };
}
