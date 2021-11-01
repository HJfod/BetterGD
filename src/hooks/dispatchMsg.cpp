#include "bgd_hook.hpp"

bool CCMouseDispatcher_dispatchScrollMSG(
    CCMouseDispatcher* self, float y, float x
) {
    if (SuperMouseManager::get()->dispatchScrollEvent(y, x, getMousePos()))
        return true;
    return matdash::orig<&CCMouseDispatcher_dispatchScrollMSG, matdash::Thiscall>(self, y, x);
}
static CreateHook<&CCMouseDispatcher_dispatchScrollMSG, matdash::Thiscall>$ccmddsm(
    "libcocos2d.dll",
    "?dispatchScrollMSG@CCMouseDispatcher@cocos2d@@QAE_NMM@Z"
);

void CCKeyboardDispatcher_dispatchKeyboardMSG(
    CCKeyboardDispatcher* self,
    enumKeyCodes key,
    bool down
) {
    if (SuperKeyboardManager::get()->dispatchEvent(key, down))
        return;
    
    return matdash::orig<&CCKeyboardDispatcher_dispatchKeyboardMSG>(self, key, down);
}
static CreateHook<&CCKeyboardDispatcher_dispatchKeyboardMSG>$cckddkm(
    "libcocos2d.dll",
    "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z"
);

void CCScheduler_update(CCScheduler* self, float dt) {
    SuperMouseManager::get()->dispatchMoveEvent(getMousePos());
    KeybindManager::get()->handleRepeats(dt);

    return matdash::orig<&CCScheduler_update, matdash::Thiscall>(self, dt);
}
static CreateHook<&CCScheduler_update, matdash::Thiscall>$ccsu(
    "libcocos2d.dll",
    "?update@CCScheduler@cocos2d@@UAEXM@Z"
);

void CCEGLView_onGLFWMouseCallBack(
    CCEGLView* self,
    GLFWwindow* wnd,
    int btn,
    int pressed,
    int z
) {
    if (SuperMouseManager::get()->dispatchClickEvent(
        static_cast<MouseButton>(btn), pressed, getMousePos()
    ))
        return;

    return matdash::orig<&CCEGLView_onGLFWMouseCallBack>(self, wnd, btn, pressed, z);
}
static CreateHook<&CCEGLView_onGLFWMouseCallBack>$ccevogmc(
    "libcocos2d.dll",
    "?onGLFWMouseCallBack@CCEGLView@cocos2d@@IAEXPAUGLFWwindow@@HHH@Z"
);
