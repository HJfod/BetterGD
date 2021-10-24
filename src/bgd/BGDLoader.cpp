#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>
#include <utils/other/platform.hpp>
#include <iostream>

bgd::BGDLoader* bgd::BGDLoader::get() {
    static auto g_loader = new BGDLoader;
    return g_loader;
}

void bgd::BGDLoader::createDirectories() {
    directory_create(bgd_directory);
    directory_create(bgd_directory + "/"_s + bgd_plugin_directory);
}

size_t bgd::BGDLoader::updatePlugins() {
    size_t loaded = 0;
    this->createDirectories();
    for (auto const& entry : std::filesystem::directory_iterator(
        std::filesystem::absolute(bgd_directory) / bgd_plugin_directory
    )) {
        if (
            std::filesystem::is_regular_file(entry) &&
            entry.path().extension() == bgd_plugin_extension
        ) {
            if (!this->m_mLoadedPlugins.count(entry.path().string())) {
                if (this->loadPluginFromFile(entry.path().string())) {
                    loaded++;
                }
            }
        }
    }
    std::cout << "plugin list: \n";
    for (auto const& [_, plugin] : this->m_mLoadedPlugins) {
        std::cout << plugin->m_sName << "\n";
    }
    return loaded;
}

bool bgd::BGDLoader::isPluginLoaded(std::string const& id) {
    return map_contains<std::string, BGDPlugin*>(
        this->m_mLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

bgd::BGDPlugin* bgd::BGDLoader::getLoadedPlugin(std::string const& id) {
    return map_select<std::string, BGDPlugin*>(
        this->m_mLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

bool bgd::BGDLoader::setup() {
    if (this->m_bIsSetup)
        return true;

    bgd::loadConsole();

    std::cout << "setting up BGDLoader\n";

    this->createDirectories();
    this->updatePlugins();

    bgd::bufferConsoleInput();

    this->m_bIsSetup = true;
    return true;
}

bgd::BGDLoader::BGDLoader() {}

bgd::BGDLoader::~BGDLoader() {
    for (auto const& [_, plugin] : this->m_mLoadedPlugins) {
        delete plugin;
    }
}
