#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>
#include <utils/other/platform.hpp>
#include <iostream>

using namespace bgd;

BGDLoader* bgd::BGDLoader::get() {
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
        std::cout << plugin << " -> " << plugin->m_sName << "\n";
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

BGDPlugin* bgd::BGDLoader::getLoadedPlugin(std::string const& id) {
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

void bgd::BGDLoader::throwError(BGDError const& error) {
    this->m_vErrors.push_back(error);
}

std::vector<BGDError> bgd::BGDLoader::getErrors(
    std::initializer_list<BGDErrorType> typeFilter,
    std::initializer_list<BGDSeverity>  severityFilter
) {
    if (
        !typeFilter.size() && !severityFilter.size()
    ) return this->m_vErrors;

    std::vector<BGDError> errs;

    for (auto const& err : this->m_vErrors) {
        for (auto const& type : typeFilter) {
            if (err.type == type) {
                errs.push_back(err);
                break;
            }
        }
        for (auto const& severity : severityFilter) {
            if (err.severity == severity) {
                errs.push_back(err);
                break;
            }
        }
    }

    return errs;
}
