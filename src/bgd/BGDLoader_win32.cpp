#include <BGDLoader.hpp>
#include <Windows.h>
#include <utils/other/ext.hpp>
#include <iostream>

bool bgd::BGDLoader::loadPluginFromFile(std::string const& path) {
    std::cout << "loading " << path << "\n";
    auto load = LoadLibraryA(path.c_str());
    if (load) {
        std::cout << "loaded, yay\n";
        auto loadFunc = reinterpret_cast<bgd::bgd_load_type>(GetProcAddress(load, "bgd_load"));
        if (!loadFunc) {
            loadFunc  = reinterpret_cast<bgd::bgd_load_type>(GetProcAddress(load, "_bgd_load@0"));
        }
        if (loadFunc) {
            std::cout << "found entry function\n";
            auto plugin = loadFunc();
            if (plugin) {
                std::cout << "entry function call succesful!\n";
                this->m_mLoadedPlugins.insert({ path, plugin });
                FreeLibrary(load);
                return true;
            }
        }
    }
    return false;
}
