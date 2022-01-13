#include "hook.hpp"
#include <CCEGLView.h>

bool __fastcall CCMouseDispatcher_dispatchScrollMSG(
    CCMouseDispatcher* self, float y, float x
) {
    if (SuperMouseManager::get()->dispatchScrollEvent(y, x, getMousePos()))
        return true;
    return self->dispatchScrollMSG(y, x);
}
static CreateHookMod<&CCMouseDispatcher_dispatchScrollMSG>$ccmddsm(
    "libcocos2d.dll",
    "?dispatchScrollMSG@CCMouseDispatcher@cocos2d@@QAE_NMM@Z"
);

bool __fastcall CCKeyboardDispatcher_dispatchKeyboardMSG(
    CCKeyboardDispatcher* self,
    edx_t,
    enumKeyCodes key,
    bool down
) {
    if (SuperKeyboardManager::get()->dispatchEvent(key, down))
        return true;
    
    return self->dispatchKeyboardMSG(key, down);
}
static CreateHookMod<&CCKeyboardDispatcher_dispatchKeyboardMSG>$cckddkm(
    "libcocos2d.dll",
    "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z"
);

void __fastcall CCScheduler_update(CCScheduler* self, edx_t, float dt) {
    SuperMouseManager::get()->dispatchMoveEvent(getMousePos());

    return self->update(dt);
}
static CreateHookMod<&CCScheduler_update>$ccsu(
    "libcocos2d.dll",
    "?update@CCScheduler@cocos2d@@UAEXM@Z"
);

class CCEGLView_hook : public CCEGLView {
public:
    static void __fastcall onGLFWMouseCallBack(
        CCEGLView_hook* egl,
        edx_t,
        GLFWwindow* wnd,
        int btn,
        int pressed,
        int z
    ) {
        if (SuperMouseManager::get()->dispatchClickEvent(
            static_cast<MouseButton>(btn), pressed, getMousePos()
        ))
            return;

        return egl->CCEGLView::onGLFWMouseCallBack(wnd, btn, pressed, z);
    }
};
static CreateHookMod<&CCEGLView_hook::onGLFWMouseCallBack>$ccevogmc(
    "libcocos2d.dll",
    "?onGLFWMouseCallBack@CCEGLView@cocos2d@@IAEXPAUGLFWwindow@@HHH@Z"
);
