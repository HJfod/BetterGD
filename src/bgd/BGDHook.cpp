#include <BGDHook.hpp>
#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

BGDHookInfo::BGDHookInfo(BGDHook* hook) : hook(hook) {
    this->module = "GeometryDash.exe";
    auto addr = as<uintptr_t>(hook->m_nAddress);
    HMODULE mod;
    if (GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        as<LPCSTR>(hook->getAddress()),
        &mod
    )) {
        addr -= as<uintptr_t>(mod);
        char name[MAX_PATH];
        if (GetModuleFileNameA(
            mod, name, sizeof name
        )) {
            this->module = std::filesystem::path(name).stem().string();
        }
    }
}

std::string bgd::BGDHookInfo::formatted() const {
    return cstrfmt(
        "%s + 0x%X",
        module.c_str(),
        this->hook->m_nAddress
    );
}

std::string bgd::BGDHookInfo::formattedDetails() const {
    return cstrfmt(
        "Detour: %p\nHandle: %p\n%s",
        this->hook->m_pDetour,
        this->hook->m_pHandle,
        this->hook->m_bEnabled ? "Enabled" : "Disabled"
    );
}
