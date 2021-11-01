#pragma once

#include "PluginListView.hpp"

class PluginLayer : public GJDropDownLayer {
    protected:
        bool init();

        void onViewErrors(CCObject*);

    public:
        static PluginLayer* create();
};
