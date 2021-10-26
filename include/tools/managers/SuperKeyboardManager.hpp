#pragma once

#include <BGDUtils.hpp>

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

            bool init();
        
        public:
            static bool initGlobal();
            static SuperKeyboardManager* get();

            void pushDelegate(SuperKeyboardDelegate*);
            void popDelegate(SuperKeyboardDelegate*);

            bool dispatchEvent(cocos2d::enumKeyCodes, bool keydown);
    };
}
