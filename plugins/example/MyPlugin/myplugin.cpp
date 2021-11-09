#include "myplugin.hpp"

USE_BGD_NAMESPACE();

void MyPlugin::setup() {
    this->m_sID = "hjfod.my_awesome_plugin";
    this->m_sName = "My Awesome Plugin";
    this->m_sDeveloper = "HJfod";
    this->m_sDescription = "Awesome example plugin";

    this->logText("Hello from my plugin!");
}

void MyPlugin::logText(std::string const& text) {
    this->log() << "Logged by MyPlugin: " << text << bgd::endl;
}

MyPlugin* MyPlugin::get() {
    static MyPlugin* g_plugin = new MyPlugin;
    return g_plugin;
}

BGD_LOAD(MyPlugin);


