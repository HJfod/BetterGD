#include <BGDLoader.hpp>
#include <Windows.h>
#include <utils/other/ext.hpp>

void bgd::BGDLoader::loadPluginFromFile(std::string const& path) {
    auto load = LoadLibraryA(path.c_str());
    if (load) {
        auto addr = reinterpret_cast<bgd::bgd_load_type>(GetProcAddress(load, "bgd_load"));

        if (addr) {
            auto plugin = addr();

            if (plugin) {
                this->m_mLoadedPlugins.insert({ path, plugin });
            }
        }
    }
}
