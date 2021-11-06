#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

class DevTools : public CCNode {
    protected:
        bool m_bVisible = false;

        void setSceneScale(CCScene*, bool transition);

        static DevTools* create();

    public:
        static DevTools* get();

        void fixSceneScale(CCScene* scene);
        void willSwitchToScene(CCScene* scene);

        void show();
        void hide();
        void toggle();
};
