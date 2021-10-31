#include "bgd_hook.hpp"
#include <lilac_hook.hpp>

struct hook_info {
    void* address;
    void* detour;
};

static std::vector<hook_info> g_hooks;

bool bgd::loadHooks() {
    auto thereWereErrors = false;
    for (auto const& hook : g_hooks) {
        if (!lilac::Hooks::add(hook.address, hook.detour)) {
            BGDLoader::get()->throwError(BGDError {
                "Error Creating Hook",
                "Unable to create hook at " + std::to_string(as<int>(hook.address)),
                kBGDSeverityError,
                kBGDErrorTypeHook,
                nullptr
            });
            thereWereErrors = true;
        }
    }
    return thereWereErrors;
}

bgd::Result<> matdash::__mat_dash_add_hook(void* addr, void* detour, void** trampoline) {
    *trampoline = addr;
    g_hooks.push_back({ addr, detour });
    return Ok<>();
}

/*
bgd::Result<> matdash::__mat_dash_add_hook(void* addr, void* detour, void** trampoline) {
    static bool initialized = false;
    if (!initialized) {
        auto init = MH_Initialize();
        if (
            init != MH_OK &&
            init != MH_ERROR_ALREADY_INITIALIZED
        ) return Err<>(
            "Unable to initialize MinHook: "_s + MH_StatusToString(init)
        );
        initialized = true;
    }
    auto hook = MH_CreateHook(addr, detour, trampoline);
    if (
        hook != MH_OK
    ) return Err<>(
        "Unable to create hook at " +
        std::to_string(as<uintptr_t>(addr)) + ": " +
        MH_StatusToString(hook)
    );

    auto enable = MH_EnableHook(addr);
    if (
        enable != MH_OK
    ) return Err<>(
        "Unable to enable hook at " + std::to_string(as<uintptr_t>(addr)) + ": " +
        MH_StatusToString(enable)
    );

    return Ok<>();
}
*/
