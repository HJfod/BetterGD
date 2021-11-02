#include "BGDInternal.hpp"
#include <BGDLoader.hpp>

using namespace std::string_view_literals;

USE_BGD_NAMESPACE();

void BGDInternal::addResourceSearchPaths() {
    CCFileUtils::sharedFileUtils()->addSearchPath(
        const_join_path_c_str<bgd_directory, bgd_resource_directory>
    );
}

void BGDInternal::setup() {
    this->addResourceSearchPaths();
}

BGDInternal* BGDInternal::get() {
    static auto g_int = new BGDInternal;
    return g_int;
}
