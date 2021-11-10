#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>
#include <BGDSaveManager.hpp>
#include <BGDLog.hpp>
#include <BGDInternal.hpp>
#include <iostream>
#include <utils/other/ext.hpp>

USE_BGD_NAMESPACE();

BGDPlugin::BGDPlugin() {
    this->setup();
}

BGDPlugin::~BGDPlugin() {
    this->platformCleanup();
    for (auto const& hook : this->m_vHooks) {
        this->removeHook(hook);
    }
}

BGDLogStream& BGDPlugin::log() {
    return BGDLoader::get()->logStream() << this;
}

void BGDPlugin::setup() {}
void BGDPlugin::enable() {}
void BGDPlugin::disable() {}
void BGDPlugin::saveData() {
    auto path = "BetterGD/plugins/"_s;
    path += this->m_sName;
    for (auto const& manager : this->m_vSaveManagers) {
        manager->saveData(path);
    }
}
void BGDPlugin::loadData() {
    auto path = "BetterGD/plugins/"_s;
    path += this->m_sName;
    for (auto const& manager : this->m_vSaveManagers) {
        manager->loadData(path);
    }
}

decltype(BGDPlugin::m_sID) BGDPlugin::getID() const {
    return this->m_sID;
}

decltype(BGDPlugin::m_sName) BGDPlugin::getName() const {
    return this->m_sName;
}

decltype(BGDPlugin::m_sDeveloper) BGDPlugin::getDeveloper() const {
    return this->m_sDeveloper;
}

decltype(BGDPlugin::m_sCredits) BGDPlugin::getCredits() const {
    return this->m_sCredits;
}

decltype(BGDPlugin::m_sDescription) BGDPlugin::getDescription() const {
    return this->m_sDescription;
}

decltype(BGDPlugin::m_sPath) BGDPlugin::getPath() const {
    return this->m_sPath;
}

std::vector<BGDHook*> BGDPlugin::getHooks() const {
    return this->m_vHooks;
}

void BGDPlugin::registerSaveManager(BGDSaveManager* manager) {
    this->m_vSaveManagers.push_back(manager);
}

void BGDPlugin::unregisterSaveManager(BGDSaveManager* manager) {
    vector_erase(this->m_vSaveManagers, manager);
}

void BGDPlugin::throwError(
    std::string_view const& info,
    BGDSeverity severity
) {
    BGDLoader::get()->log(new BGDLogMessage(
        std::string(info),
        severity,
        kBGDLogTypeError,
        this
    ));
}
