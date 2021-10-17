#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>

using namespace bgd;

BGDLoader* BGDLoader::get() {
    static auto g_loader = new BGDLoader;
    return g_loader;
}

bool BGDLoader::isPluginLoaded(std::string const& id) {
    return ext::vector(this->m_vLoadedPlugins).contains()
}

BGDLoader::BGDLoader() {}

BGDLoader::~BGDLoader() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        delete plugin;
    }
}
