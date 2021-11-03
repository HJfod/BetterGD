#pragma once

#include <BetterGD.hpp>

USE_BGD_NAMESPACE();

class PluginInfoLayer : public BrownAlertDelegate {
    protected:
        BGDPlugin* m_pPlugin;

        void setup() override;

        void onHooks(CCObject*);
    
    public:
        static PluginInfoLayer* create(BGDPlugin* plugin);
};
