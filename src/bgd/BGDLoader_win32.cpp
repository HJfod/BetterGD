#include <BGDLoader.hpp>

#ifdef BGD_IS_WIN32

#include <Windows.h>
#include <utils/other/ext.hpp>
#include <utils/other/platform.hpp>
#include <iostream>

bool bgd::BGDLoader::loadPluginFromFile(std::string const& path) {
    auto load = LoadLibraryA(path.c_str());
    if (load) {
        auto loadFunc = reinterpret_cast<bgd::bgd_load_type>(GetProcAddress(load, "bgd_load"));
        if (!loadFunc) {
            loadFunc  = reinterpret_cast<bgd::bgd_load_type>(GetProcAddress(load, "_bgd_load@0"));
        }
        if (loadFunc) {
            auto plugin = loadFunc();
            if (plugin) {
                plugin->setup();
                plugin->m_pInfo = new bgd::BGDPlatformInfo { load };
                this->m_mLoadedPlugins.insert({ path, plugin });
                return true;
            }
        }
    }
    return false;
}

#endif
