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

            this->log() << "hey from other plugin" << bgd::endl;
        }
        BGD_PLUGIN_GET(OtherPlugin);
};









bool GJGarageLayer_init(GJGarageLayer* self) {
    if (!bgd::hook::orig<&GJGarageLayer_init>(self))
        return false;
    
    if (BGDLoader::get()->isPluginLoaded("hjfod.my_awesome_plugin")) {
        MyPlugin::get()->logText("Hello from other plugin!");
    } else {
        OtherPlugin::get()->log() << "MyPlugin is not loaded :(" << bgd::endl;
    }

    // log a CCNode to the console
    OtherPlugin::get()->log() << "hey check out this node: " << getChild<CCNode*>(self, 6) << bgd::endl;
    
    // log GJGarageLayer to the console
    OtherPlugin::get()->log() << self << bgd::endl;

    // log the plugin itself (empty log)
    OtherPlugin::get()->log() << bgd::endl;

    return true;
}
static CreateHook<&GJGarageLayer_init>$gjgli(base + 0x1255d0, OtherPlugin::get());










BGD_LOAD(OtherPlugin);


