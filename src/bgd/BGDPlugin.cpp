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

void bgd::BGDPlugin::throwError(bgd::BGDError const& error) {
    auto err = error;
    err.plugin = this;
    bgd::BGDLoader::get()->throwError(err);
}
