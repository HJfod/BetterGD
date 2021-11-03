#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>
#include <BGDSaveManager.hpp>
#include <iostream>
#include <utils/other/ext.hpp>

bgd::BGDPlugin::BGDPlugin() {
    this->setup();
}

bgd::BGDPlugin::~BGDPlugin() {
    this->platformCleanup();
    for (auto const& hook : this->m_vHooks) {
        this->removeHook(hook);
    }
}

std::ostream& bgd::BGDPlugin::log() {
    return std::cout << " [ " << this->m_sName << " ]: ";
}

void bgd::BGDPlugin::setup() {}
void bgd::BGDPlugin::enable() {}
void bgd::BGDPlugin::disable() {}
void bgd::BGDPlugin::saveData() {
    auto path = "BetterGD/plugins/"_s + this->m_sName;
    for (auto const& manager : this->m_vSaveManagers) {
        manager->saveData(path);
    }
}
void bgd::BGDPlugin::loadData() {
    auto path = "BetterGD/plugins/"_s + this->m_sName;
    for (auto const& manager : this->m_vSaveManagers) {
        manager->loadData(path);
    }
}

const char* bgd::BGDPlugin::getID()        const {
    return this->m_sID;
}

const char* bgd::BGDPlugin::getName()      const {
    return this->m_sName;
}

const char* bgd::BGDPlugin::getDeveloper() const {
    return this->m_sDeveloper;
}

const char* bgd::BGDPlugin::getCredits()   const {
    return this->m_sCredits;
}

const char* bgd::BGDPlugin::getPath()      const {
    return this->m_sPath;
}

void bgd::BGDPlugin::registerSaveManager(BGDSaveManager* manager) {
    this->m_vSaveManagers.push_back(manager);
}

void bgd::BGDPlugin::unregisterSaveManager(BGDSaveManager* manager) {
    vector_erase(this->m_vSaveManagers, manager);
}

void bgd::BGDPlugin::throwError(bgd::BGDError const& error) {
    auto err = error;
    err.plugin = this;
    bgd::BGDLoader::get()->throwError(err);
}
