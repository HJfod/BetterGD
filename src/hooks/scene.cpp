#include "bgd_hook.hpp"
#include "../devtools/DevTools.hpp"

void AppDelegate_willSwitchToScene(CCNode* self, CCScene* toScene) {
    hook::orig<&AppDelegate_willSwitchToScene>(self, toScene);
    DevTools::get()->willSwitchToScene(toScene);
}
static InternalCreateHook<&AppDelegate_willSwitchToScene>$adwsts(base + 0x3d690);

void CCTransitionScene_finish(CCTransitionScene* self) {
    hook::orig<&CCTransitionScene_finish>(self);
    DevTools::get()->fixSceneScale(self);
}
static InternalCreateHook<&CCTransitionScene_finish>$cctsf(
    "libcocos2d.dll",
    "?finish@CCTransitionScene@cocos2d@@QAEXXZ"
);
