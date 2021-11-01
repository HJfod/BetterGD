#pragma once

#include <BGDUtils.hpp>
#include "BrownAlertDelegate.hpp"

namespace bgd {
    class BGD_DLL PickMenu : public BrownAlertDelegate {
        protected:
            using picks_t = std::vector<std::pair<std::string, cocos2d::SEL_MenuHandler>>;

            picks_t m_mPicks;
            cocos2d::CCObject* m_pThis;

            void setup() override;
            
            void onSelect(cocos2d::CCObject*);

            bool initPicks(const char*, cocos2d::CCObject*, picks_t const&);

        public:
            static PickMenu * create(const char*, cocos2d::CCObject*, picks_t const&);
    };
}
