#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>
#include <iostream>

bgd::BGDPlugin::BGDPlugin() {
    this->setup();
}

bgd::BGDPlugin::~BGDPlugin() {
    this->platformCleanup();
}

std::ostream& bgd::BGDPlugin::log() {
    return std::cout << " [ " << this->m_sName << " ]: ";
}

void bgd::BGDPlugin::setup() {}
void bgd::BGDPlugin::enable() {}
void bgd::BGDPlugin::disable() {}
void bgd::BGDPlugin::saveData() {}
void bgd::BGDPlugin::loadData() {}

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

void bgd::BGDPlugin::throwError(bgd::BGDError const& error) {
    auto err = error;
    err.plugin = this;
    bgd::BGDLoader::get()->throwError(err);
}
