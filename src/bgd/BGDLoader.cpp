#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>
#include <iostream>

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
            if (!this->m_pLoadedPlugins->count(file.string())) {
                this->loadPluginFromFile(file.string());
            }
        }
    }
    std::cout << "plugin list: \n";
    for (auto const& [_, plugin] : *this->m_pLoadedPlugins) {
        std::cout << plugin->m_sName << "\n";
    }
}

bool bgd::BGDLoader::isPluginLoaded(std::string const& id) {
    return map_contains<std::string, BGDPlugin*>(
        *this->m_pLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

bool bgd::BGDLoader::setup() {
    if (this->m_bIsSetup)
        return true;

    std::cout << "setting up BGDLoader\n";
    
    this->createDirectories();

    this->m_bIsSetup = true;
    return true;
}

bgd::BGDPlugin* bgd::BGDLoader::getLoadedPlugin(std::string const& id) {
    return map_select<std::string, BGDPlugin*>(
        *this->m_pLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

bgd::BGDLoader::BGDLoader() {
    this->m_pLoadedPlugins = new std::unordered_map<std::string, BGDPlugin*>;
}

bgd::BGDLoader::~BGDLoader() {
    for (auto const& [_, plugin] : *this->m_pLoadedPlugins) {
        delete plugin;
    }

    delete this->m_pLoadedPlugins;
}
