#include "BGDInternal.hpp"
#include <BetterGD.hpp>

using namespace std::string_view_literals;

USE_BGD_NAMESPACE();

void BGDInternal::addResourceSearchPaths() {
    auto utils = CCFileUtils::sharedFileUtils();
    auto path = utils->fullPathForFilename(const_join_path_c_str<bgd_directory, bgd_resource_directory>, false);
    if (!vector_utils::contains<std::string>(utils->getSearchPaths(), path)) {
        utils->addSearchPath(path.c_str());
    }
}

void BGDInternal::loadKeybinds() {
}

bool BGDInternal::isFileInSearchPaths(const char* file) {
    auto utils = CCFileUtils::sharedFileUtils();
    return utils->isFileExist(utils->fullPathForFilename(file, false));
}

void BGDInternal::setup() {
    this->loadKeybinds();
    this->addResourceSearchPaths();
}

BGDInternal* BGDInternal::get() {
    static auto g_int = new BGDInternal;
    return g_int;
}
