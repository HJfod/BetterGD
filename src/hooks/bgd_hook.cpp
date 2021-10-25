#include "bgd_hook.hpp"
#include <Minhook.h>

bgd::Result<> matdash::__mat_dash_add_hook(void* addr, void* detour, void** trampoline) {
    static bool initialized = false;
    if (!initialized) {
        auto init = MH_Initialize();
        if (
            init != MH_OK &&
            init != MH_ERROR_ALREADY_INITIALIZED
        ) return Err<>("Unable to initialize MinHook");
        initialized = true;
    }
    auto hook = MH_CreateHook(addr, detour, trampoline);
    if (
        hook != MH_OK
    ) return Err<>("Unable to create hook at " + std::to_string(as<uintptr_t>(addr)));

    auto enable = MH_EnableHook(addr);
    if (
        enable != MH_OK
    ) return Err<>("Unable to enable hook at " + std::to_string(as<uintptr_t>(addr)));

    return Ok<>();
}
