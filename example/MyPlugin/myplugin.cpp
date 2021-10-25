#include "myplugin.hpp"

USE_BGD_NAMESPACE();

void MyPlugin::setup() {
    this->m_sID = "hjfod.my_awesome_plugin";
    this->m_sName = "My Awesome Plugin";

    this->logText("Hello from my plugin!");
}

void MyPlugin::logText(std::string const& text) {
    std::cout << "Logged by MyPlugin: " << text << "\n";
}

MyPlugin* MyPlugin::get() {
    static MyPlugin* g_plugin = new MyPlugin;
    return g_plugin;
}

extern "C" {
    __declspec(dllexport) bgd::BGDPlugin* __stdcall bgd_load() {
        std::cout << "wee woo i have been called!\n";
        auto ret = MyPlugin::get();
        std::cout << "MyPlugin is: " << ret << "\n";
        return ret;
    }
};
// BGD_LOAD(MyPlugin);


