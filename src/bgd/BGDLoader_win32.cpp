#include <BGDLoader.hpp>
#include <Windows.h>
#include <utils/other/ext.hpp>
#include <iostream>

void bgd::BGDLoader::loadPluginFromFile(std::string const& path) {
    std::cout << "loading " << path << "\n";
    auto load = LoadLibraryA(path.c_str());
    if (load) {
        auto addr = reinterpret_cast<bgd::bgd_load_type>(GetProcAddress(load, "bgd_load"));
        if (addr) {
            std::cout << "found entry function\n";
            auto plugin = addr();
            if (plugin) {
                this->m_pLoadedPlugins->insert({ path, plugin });
            }
        }
    }
}
