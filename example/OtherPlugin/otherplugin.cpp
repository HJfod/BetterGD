#include <BetterGD.hpp>
#include "../MyPlugin/myplugin.hpp"

USE_BGD_NAMESPACE();

class OtherPlugin : public bgd::BGDPlugin {
    protected:
        void setup() override {
            this->m_sID = "hjfod.other_awesome_plugin";
            this->m_sName = "My Other Plugin";
            this->m_sDeveloper = "HJfod";
            this->m_sCredits = "Various different chords in Onnanoko ni Naritai";

            std::cout << "hey from other plugin\n";
        }
        BGD_PLUGIN_GET(OtherPlugin);
};

bool GJGarageLayer_init(GJGarageLayer* self) {
    if (!matdash::orig<&GJGarageLayer_init>(self))
        return false;
    
    if (BGDLoader::get()->isPluginLoaded("hjfod.my_awesome_plugin")) {
        MyPlugin::get()->logText("Hello from other plugin!");
    } else {
        std::cout << "MyPlugin is not loaded :( T: other plugin\n";
    }

    return true;
}
static CreateHook<&GJGarageLayer_init>$gjgli(base + 0x1255d0, OtherPlugin::get());

BGD_LOAD(OtherPlugin);


