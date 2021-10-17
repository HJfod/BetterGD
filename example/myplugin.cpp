#include "../include/BetterGD.hpp"

class MyPlugin : public bgd::BGDPlugin {
    protected:
        void setup() override {
            this->m_sID = "hjfod.my_awesome_plugin";
            this->m_sName = "My Awesome Plugin";
        }
        // A `get` method is required for the
        // `BGD_LOAD` macro, and is useful in
        // general in case you need to access
        // your plugin instance elsewhere in
        // the code.

        // Note that if your plugin interface
        // is available for other mods, this
        // should be defined in a source file
        // instead in order to make sure
        // the other mod doesn't generate its
        // own instance of your plugin.
        BGD_PLUGIN_GET(MyPlugin);
};

BGD_LOAD(MyPlugin);


