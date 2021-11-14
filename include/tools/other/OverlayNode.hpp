#pragma once

#include <BGDUtils.hpp>

namespace bgd {
    /** OverlayNode
     * @details A node that always inhabits CCDirector::notificationNode.
     * Use this to draw CCNodes over the current CCScene.
     */
    class BGD_DLL OverlayNode : public cocos2d::CCNode {
        protected:
            bool init();

            void moveCurrentNoficiationNode();
        
            static OverlayNode* create();

        public:
            static OverlayNode* get();
    };
}
