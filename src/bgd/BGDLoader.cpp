#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>

bgd::BGDLoader* bgd::BGDLoader::get() {
    static auto g_loader = new BGDLoader;
    return g_loader;
}

void bgd::BGDLoader::createDirectories() {
    directory_create(bgd_directory);
    directory_create(bgd_directory + "/"_s + bgd_plugin_directory);
}

void bgd::BGDLoader::updatePlugins() {
    this->createDirectories();
    auto baseDir = std::filesystem::path(bgd_directory);
    for (auto const& filer : baseDir / bgd_plugin_directory) {
        auto file = std::filesystem::absolute(baseDir / filer);
        if (
            std::filesystem::is_regular_file(file) &&
            file.extension() == bgd_plugin_extension
        ) {
            if (!this->m_mLoadedPlugins.count(file.string())) {
                this->loadPluginFromFile(file.string());
            }
        }
    }
}

bool bgd::BGDLoader::isPluginLoaded(std::string const& id) {
    return map_contains<std::string, BGDPlugin*>(
        this->m_mLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            p->m_sID == id;
        }
    );
}

bgd::BGDPlugin* bgd::BGDLoader::getLoadedPlugin(std::string const& id) {
    return map_select<std::string, BGDPlugin*>(
        this->m_mLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            p->m_sID == id;
        }
    );
}

bgd::BGDLoader::BGDLoader() {}

bgd::BGDLoader::~BGDLoader() {
    for (auto const& [_, plugin] : this->m_mLoadedPlugins) {
        delete plugin;
    }
}
