#pragma once

#include "HookListView.hpp"

class HookListLayer : public GJDropDownLayer {
    protected:
        bool init(BGDPlugin* plugin);

    public:
        static HookListLayer* create(BGDPlugin* plugin);
};
