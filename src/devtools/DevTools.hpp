#pragma once

#include <BGDUtils.hpp>
#include "ui/NativeUI.hpp"

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
        NativeUI* m_ui;

        void showAnimation(CCScene*, bool transition);
        void loadColorScheme();
        void recurseUpdateList(CCNode* parent);

        void draw();

        DevTools();
        ~DevTools();

    public:
        static DevTools* get();

        NativeUI* ui();

        void fixSceneScale(CCScene* scene);
        void willSwitchToScene(CCScene* scene);

        void show();
        void hide();
        void toggle();
};
