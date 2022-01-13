#pragma once

#include <BGDUtils.hpp>

namespace bgd {
    class BGD_DLL BGCheckbox : public cocos2d::CCNode {
        protected:
            cocos2d::extension::CCScale9Sprite* m_pBGLayer;
            CCMenuItemToggler* m_pToggler;
            cocos2d::CCLabelBMFont* m_pLabel;

            bool init(const char*);
        
        public:
            static BGCheckbox* create(const char*);

            void setEnabled(bool);

            inline CCMenuItemToggler* getToggle() { return m_pToggler; }
    };
}
