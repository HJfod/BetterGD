#include <BGDMacros.hpp>
#include <BGDPlugin.hpp>

#ifdef BGD_IS_WIN32

#include <utils/other/platform.hpp>

void bgd::BGDPlugin::platformCleanup() {
    // pretty sure this is unnecessary...
    // FreeLibrary frees up the memory
    // associated with m_pInfo anyway
    // I think
    auto hmod = this->m_pInfo->hmod;
    delete this->m_pInfo;
    FreeLibrary(hmod);
}

#endif
