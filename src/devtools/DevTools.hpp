#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

enum DevToolsMode {
    kDevToolsModeIntegrated,
    kDevToolsModePopup,
};

enum DevToolsMount {
    kDevToolsMountWest,
    kDevToolsMountEast,
    kDevToolsMountNorth,
    kDevToolsMountSouth,
};

class DevTools {
    protected:
        bool m_bVisible = false;
        float m_fWidth  = 200.f;
        float m_fHeight = 100.f;
        float m_fPadding= 20.f;
        CCSize m_obPopoutSize = { 200.f, 200.f };
        DevToolsMode m_eMode = kDevToolsModeIntegrated;
        DevToolsMount m_eMount = kDevToolsMountWest;

        void showAnimation(CCScene*, bool transition);
        void loadColorScheme();
        void recurseUpdateList(CCNode* parent);
        void generateContent();
        float getSceneScale();
        void resizeWindow();
        void updateVisibility(DevToolsMode, DevToolsMount = kDevToolsMountWest);

        static void draw();

        DevTools();
        ~DevTools();

    public:
        static DevTools* get();

        void fixSceneScale(CCScene* scene);
        void willSwitchToScene(CCScene* scene);

        void show();
        void hide();
        void toggle();
};
