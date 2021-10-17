#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>

bgd::BGDLoader* bgd::BGDLoader::get() {
    static auto g_loader = new BGDLoader;
    return g_loader;
}

void bgd::BGDLoader::createDirectories() {
    
}

void bgd::BGDLoader::updatePlugins() {
    this->createDirectories();
}

bool bgd::BGDLoader::isPluginLoaded(std::string const& id) {
    return vector_contains<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            p->m_sID == id;
        }
    );
}

bgd::BGDPlugin* bgd::BGDLoader::getLoadedPlugin(std::string const& id) {
    return vector_select<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            p->m_sID == id;
        }
    );
}

bgd::BGDLoader::BGDLoader() {}

bgd::BGDLoader::~BGDLoader() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        delete plugin;
    }
}
