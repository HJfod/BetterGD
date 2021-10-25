#pragma once

#include "PluginListView.hpp"

class PluginLayer : public GJDropDownLayer {
    protected:
        bool init();

    public:
        static PluginLayer* create();
};
