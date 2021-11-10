#include "../hooks/bgd_hook.hpp"
#include <BGDLoader.hpp>
#include <utils/other/ext.hpp>
#include <utils/other/platform.hpp>
#include <iostream>
#include <thread>
#include "BGDInternal.hpp"

USE_BGD_NAMESPACE();

BGDLoader* BGDLoader::get() {
    static auto g_loader = new BGDLoader;
    return g_loader;
}

void BGDLoader::createDirectories() {
    directory_create(const_join_path_c_str<bgd_directory>);
    directory_create(const_join_path_c_str<bgd_directory, bgd_resource_directory>);
    directory_create(const_join_path_c_str<bgd_directory, bgd_plugin_directory>);
}

size_t BGDLoader::updatePlugins() {
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

bool BGDLoader::isPluginLoaded(std::string const& id) {
    return vector_contains<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

BGDPlugin* BGDLoader::getLoadedPlugin(std::string const& id) {
    return vector_select<BGDPlugin*>(
        this->m_vLoadedPlugins,
        [id](BGDPlugin* p) -> bool {
            return p->m_sID == id;
        }
    );
}

std::vector<BGDPlugin*> BGDLoader::getAllPlugins() {
    return this->m_vLoadedPlugins;
}

bool BGDLoader::setup() {
    if (this->m_bIsSetup)
        return true;

    BGD_PLATFORM_CONSOLE(
        loadConsole();
    )
    this->createDirectories();
    BGDInternal::get()->setup();
    this->updatePlugins();
    BGDInternal::get()->loadHooks();
    this->loadData();

    this->m_bIsSetup = true;

    return true;
}

void BGDLoader::loadData() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        plugin->loadData();
    }
}

void BGDLoader::saveData() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        plugin->saveData();
    }
}

BGDLoader::BGDLoader() {
    this->m_pLogStream = new BGDLogStream;
}

BGDLoader::~BGDLoader() {
    for (auto const& plugin : this->m_vLoadedPlugins) {
        delete plugin;
    }
    for (auto const& log : this->m_vLogs) {
        delete log;
    }
    delete this->m_pLogStream;
}

void BGDLoader::log(BGDLogMessage* log) {
    this->m_vLogs.push_back(log);
}

std::vector<BGDLogMessage*> const& BGDLoader::getLogs() const {
    return this->m_vLogs;
}

std::vector<BGDLogMessage*> BGDLoader::getLogs(
    std::initializer_list<BGDLogType>  typeFilter,
    std::initializer_list<BGDSeverity> severityFilter
) {
    if (
        !typeFilter.size() && !severityFilter.size()
    ) {
        return this->m_vLogs;
    }

    std::vector<BGDLogMessage*> logs;

    for (auto const& log : this->m_vLogs) {
        for (auto const& type : typeFilter) {
            if (log->getType() == type) {
                logs.push_back(log);
                goto found_this;
            }
        }
        for (auto const& severity : severityFilter) {
            if (log->getSeverity() == severity) {
                logs.push_back(log);
                goto found_this;
            }
        }
        found_this:;
    }

    return logs;
}
