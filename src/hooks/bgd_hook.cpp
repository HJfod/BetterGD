#include "bgd_hook.hpp"
#include <lilac_hook.hpp>

struct hook_info {
    BGDHook* hook;
    BGDPlugin* plugin;
};

static std::vector<hook_info> g_hooks;
static bool g_bReadyToHook = false;

Result<BGDHook*> BGDPluginBase::addHookBase(void* addr, void* detour, BGDHook* hook) {
    if (!hook) {
        hook = new BGDHook(addr);
    }
    if ((hook->m_pHandle = const_cast<void*>(lilac::Hooks::add(addr, detour)))) {
        this->m_vHooks.push_back(hook);
        hook->m_bEnabled = true;
        return Ok<BGDHook*>(hook);
    } else {
        delete hook;
        return Err<>(
            "Unable to create hook at " + std::to_string(as<uintptr_t>(addr))
        );
    }
}

Result<BGDHook*> BGDPluginBase::addHookBase(BGDHook* hook) {
    return this->addHookBase(
        hook->m_nAddress,
        hook->m_pDetour,
        hook
    );
}

bgd::Result<> BGDPlugin::enableHook(BGDHook* hook) {
    if (!hook->isEnabled()) {
        if (!hook->m_pHandle) {
            if ((hook->m_pHandle = const_cast<void*>(lilac::Hooks::add(hook->m_nAddress, hook->m_pDetour)))) {
                hook->m_bEnabled = true;
                return Ok<>();
            }
            return Err<>("Unable to create hook");
        }
        return Err<>("Hook already has a handle");
    }
    return Ok<>();
}

bgd::Result<> BGDPlugin::disableHook(BGDHook* hook) {
    if (hook->isEnabled()) {
        if (hook->m_pHandle) {
            if (lilac::Hooks::remove(hook->m_pHandle)) {
                hook->m_bEnabled = false;
                hook->m_pHandle = nullptr;
                return Ok<>();
            }
            return Err<>("Unable to remove hook");
        }
        return Err<>("Hook lacks a handle");
    }
    return Ok<>();
}

bgd::Result<> BGDPlugin::removeHook(BGDHook* hook) {
    auto res = this->disableHook(hook);
    if (res) {
        vector_erase<BGDHook*>(this->m_vHooks, hook);
        delete hook;
    }
    return res;
}

bool BGDInternal::loadHooks() {
    std::cout << __FUNCTION__ << "\n";
    auto thereWereErrors = false;
    for (auto const& hook : g_hooks) {
        auto res = hook.plugin->addHookBase(hook.hook);
        if (!res) {
            hook.plugin->throwError(
                "Error Creating Hook",
                res.error(),
                kBGDSeverityError,
                kBGDErrorTypeHook
            );
            thereWereErrors = true;
        }
    }
    g_bReadyToHook = true;
    return thereWereErrors;
}

Result<BGDHook*> BGDPlugin::addHookInternal(void* addr, void* detour, void** trampoline) {
    *trampoline = addr;
    if (g_bReadyToHook) {
        return this->addHookBase(addr, detour);
    } else {
        auto hook = new BGDHook(addr);
        hook->m_pDetour = detour;
        g_hooks.push_back({ hook, this });
        return Ok<BGDHook*>(hook);
    }
}
