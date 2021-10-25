#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>

bgd::BGDPlugin::BGDPlugin() {
    this->setup();
}

bgd::BGDPlugin::~BGDPlugin() {
    this->platformCleanup();
}

void bgd::BGDPlugin::setup() {}
void bgd::BGDPlugin::enable() {}
void bgd::BGDPlugin::disable() {}

void bgd::BGDPlugin::throwError(bgd::BGDError const& error) {
    auto err = error;
    err.plugin = this;
    bgd::BGDLoader::get()->throwError(err);
}
