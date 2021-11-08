#include "bgd_hook.hpp"
#include "../devtools/DevTools.hpp"

bool CCMouseDispatcher_dispatchScrollMSG(
    CCMouseDispatcher* self, float y, float x
) {
    if (SuperMouseManager::get()->dispatchScrollEvent(y, x, getMousePos()))
        return true;
    return hook::orig<&CCMouseDispatcher_dispatchScrollMSG, hook::Thiscall>(self, y, x);
}
static InternalCreateHook<&CCMouseDispatcher_dispatchScrollMSG, hook::Thiscall>$ccmddsm(
    "libcocos2d.dll",
    "?dispatchScrollMSG@CCMouseDispatcher@cocos2d@@QAE_NMM@Z"
);

void CCKeyboardDispatcher_dispatchKeyboardMSG(
    CCKeyboardDispatcher* self,
    enumKeyCodes key,
    bool down
) {
    if (!keyIsModifier(key)) {
        if (KeybindManager::get()->handleGlobalKeyEvent(Keybind(key), down))
            return;
    }

    if (SuperKeyboardManager::get()->dispatchEvent(key, down))
        return;
    
    return hook::orig<&CCKeyboardDispatcher_dispatchKeyboardMSG>(self, key, down);
}
static InternalCreateHook<&CCKeyboardDispatcher_dispatchKeyboardMSG>$cckddkm(
    "libcocos2d.dll",
    "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z"
);

void CCScheduler_update(CCScheduler* self, float dt) {
    SuperMouseManager::get()->dispatchMoveEvent(getMousePos());
    KeybindManager::get()->handleRepeats(dt);

    return hook::orig<&CCScheduler_update, hook::Thiscall>(self, dt);
}
static InternalCreateHook<&CCScheduler_update, hook::Thiscall>$ccsu(
    "libcocos2d.dll",
    "?update@CCScheduler@cocos2d@@UAEXM@Z"
);

void CCDirector_drawScene(CCDirector* self) {
    hook::orig<&CCDirector_drawScene>(self);
    DevTools::get()->draw();
}
static InternalCreateHook<&CCDirector_drawScene>$ccdds(
    "libcocos2d.dll",
    "?drawScene@CCDirector@cocos2d@@QAEXXZ"
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

    return hook::orig<&CCEGLView_onGLFWMouseCallBack>(self, wnd, btn, pressed, z);
}
static InternalCreateHook<&CCEGLView_onGLFWMouseCallBack>$ccevogmc(
    "libcocos2d.dll",
    "?onGLFWMouseCallBack@CCEGLView@cocos2d@@IAEXPAUGLFWwindow@@HHH@Z"
);
