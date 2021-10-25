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
            if (!vector_contains<BGDPlugin*>(
                this->m_vLoadedPlugins,
                [entry](BGDPlugin* p) -> bool {
                    return p->m_sPath == entry.path().string();
                }
            )) {
                if (this->loadPluginFromFile(entry.path().string())) {
                    loaded++;
                }
            }
        }
    }
    return loaded;
}

bool bgd::BGDLoader::isPluginLoaded(std::string const& id) {
    return vector_contains<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

BGDPlugin* bgd::BGDLoader::getLoadedPlugin(std::string const& id) {
    return vector_select<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

std::vector<BGDPlugin*> bgd::BGDLoader::getAllPlugins() {
    return this->m_vLoadedPlugins;
}

bool bgd::BGDLoader::setup() {
    if (this->m_bIsSetup)
        return true;

    bgd::loadConsole();

    this->createDirectories();
    this->updatePlugins();

    bgd::bufferConsoleInput();

    this->loadData();

    this->m_bIsSetup = true;
    return true;
}

void bgd::BGDLoader::loadData() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        plugin->loadData();
    }
}

void bgd::BGDLoader::saveData() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        plugin->saveData();
    }
}

bgd::BGDLoader::BGDLoader() {}

bgd::BGDLoader::~BGDLoader() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
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
