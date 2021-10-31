#pragma once

#include <BGDUtils.hpp>
#include <unordered_set>
#include "../keybinds/MouseButton.hpp"

namespace bgd {

    class SuperMouseManager;

    class BGD_DLL SuperMouseDelegate {
        protected:
            cocos2d::CCPoint m_obSuperMouseHitOffset = cocos2d::CCPointZero;
            cocos2d::CCSize m_obSuperMouseHitSize = cocos2d::CCSizeZero;
            bool m_bSuperMouseHovered = false;
            bool m_bSuperMouseDown = false;

            SuperMouseDelegate();
            virtual ~SuperMouseDelegate();

            void superMousePushSelf();
            void superMousePopSelf();

            friend class SuperMouseManager;

        public:
            virtual void mouseEnterSuper(cocos2d::CCPoint const&);
            virtual void mouseLeaveSuper(cocos2d::CCPoint const&);
            virtual bool mouseDownSuper(MouseButton, cocos2d::CCPoint const&);
            virtual bool mouseUpSuper(MouseButton, cocos2d::CCPoint const&);
            virtual void mouseMoveSuper(cocos2d::CCPoint const&);
            virtual void mouseDownOutsideSuper(MouseButton, cocos2d::CCPoint const&);
            virtual bool mouseScrollSuper(float y, float x);
            virtual void mouseScrollOutsideSuper(float y, float x);
            virtual void setSuperMouseHitSize(cocos2d::CCSize const&);
            virtual void setSuperMouseHitOffset(cocos2d::CCPoint const&);
    };
    
    class BGD_DLL SuperMouseManager {
        protected:
            std::vector<SuperMouseDelegate*> m_vDelegates;
            cocos2d::CCPoint m_obLastPosition;
            SuperMouseDelegate* m_pCapturing = nullptr;
            SuperMouseDelegate* m_pWeakCapture = nullptr;
            std::unordered_set<MouseButton> m_vPressedButtons;

            bool init();

            bool delegateIsHovered(SuperMouseDelegate*, cocos2d::CCPoint const&);

            SuperMouseManager();
            ~SuperMouseManager();
        
        public:
            static SuperMouseManager* get();

            void pushDelegate(SuperMouseDelegate*);
            void popDelegate(SuperMouseDelegate*);

            void captureMouse(SuperMouseDelegate*, bool weak = false);
            void releaseCapture(SuperMouseDelegate*);
            void releaseCapture();

            bool dispatchClickEvent(MouseButton, bool, cocos2d::CCPoint const&);
            void dispatchMoveEvent(cocos2d::CCPoint const&);
            bool dispatchScrollEvent(float, float, cocos2d::CCPoint const&);

            bool isButtonPressed(MouseButton btn) const;
    };

}
