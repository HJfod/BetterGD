#include "bgd_hook.hpp"
#include <lilac_hook.hpp>

struct hook_info {
    void* address;
    void* detour;
    BGDPlugin* plugin;
};

static std::vector<hook_info> g_hooks;
static bool g_bReadyToHook = false;

Result<> BGDInternal::addHook(void* addr, void* detour, BGDPlugin* owner) {
    if (lilac::Hooks::add(addr, detour)) {
        owner->addHook(new BGDHook(as<address_t>(addr), true));
    } else {
        return Err<>(
            "Unable to create hook at " + std::to_string(as<uintptr_t>(addr))
        );
    }
    return Ok<>();
}

bool bgd::loadHooks() {
    auto thereWereErrors = false;
    for (auto const& hook : g_hooks) {
        auto res = BGDInternal::get()->addHook(hook.address, hook.detour, hook.plugin);
        if (!res) {
            BGDLoader::get()->throwError(BGDError {
                "Error Creating Hook",
                res.error(),
                kBGDSeverityError,
                kBGDErrorTypeHook,
                hook.plugin
            });
            thereWereErrors = true;
        }
    }
    return thereWereErrors;
}

Result<> hook::__mat_dash_add_hook(void* addr, void* detour, void** trampoline, BGDPlugin* owner) {
    *trampoline = addr;
    if (g_bReadyToHook) {
        return BGDInternal::get()->addHook(addr, detour, owner);
    } else {
        g_hooks.push_back({ addr, detour, owner });
    }
    return Ok<>();
}

/*
bgd::Result<> bgd::hook::__mat_dash_add_hook(void* addr, void* detour, void** trampoline) {
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
