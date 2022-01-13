#pragma once

#include <BGDUtils.hpp>
#include <unordered_set>

namespace bgd {
    struct BGD_DLL SuperKeyboardDelegate {
        virtual bool keyDownSuper(cocos2d::enumKeyCodes);
        virtual bool keyUpSuper(cocos2d::enumKeyCodes);

        SuperKeyboardDelegate();
        virtual ~SuperKeyboardDelegate();

        protected:
            void superKeyPushSelf();
            void superKeyPopSelf();
    };

    class BGD_DLL SuperKeyboardManager {
        protected:
            std::vector<SuperKeyboardDelegate*> m_vDelegates;
            std::unordered_set<cocos2d::enumKeyCodes> m_vPressedKeys;

            bool init();

            SuperKeyboardManager();
            ~SuperKeyboardManager();
        
        public:
            static SuperKeyboardManager* get();

            void pushDelegate(SuperKeyboardDelegate*);
            void popDelegate(SuperKeyboardDelegate*);

            bool dispatchEvent(cocos2d::enumKeyCodes, bool keydown);

            bool isKeyDown(cocos2d::enumKeyCodes key) const;
    };
}
