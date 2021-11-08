#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

class DevTools {
    protected:
        bool m_bVisible = false;
        float m_fWidth  = 200.f;
        float m_fHeight = 100.f;
        float m_fPadding= 20.f;
        ccColor4B m_obBGColor;
        ccColor4B m_obTextColor;
        ccColor4B m_obHoverColor;

        void showAnimation(CCScene*, bool transition);
        void loadColorScheme();
        void recurseUpdateList(CCNode* parent);

        DevTools();
        ~DevTools();

    public:
        static DevTools* get();

        void fixSceneScale(CCScene* scene);
        void willSwitchToScene(CCScene* scene);

        void draw();

        void show();
        void hide();
        void toggle();
};
