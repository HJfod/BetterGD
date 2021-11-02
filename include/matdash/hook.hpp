#pragma once

#include "matdash.hpp"
#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>

namespace bgd {
    #define CREATE_HOOK(func, abs_addr) \
        static bgd::CreateHook<&func> __STR_CAT__($,__LINE__) (abs_addr);

    #define CREATE_HOOK_GD(func, addr) \
        static bgd::CreateHook<&func> __STR_CAT__($,__LINE__) (gd::base + addr);

    template<auto Func, typename CallConv = bgd::hook::Optcall>
    struct CreateHook {
        private:
            static inline std::unordered_map<std::string, HMODULE> m_mods = {};
            
        public:
            CreateHook(uintptr_t addr, bgd::BGDPlugin* plugin) {
                static_assert(
                    !std::is_null_pointer<plugin>::value,
                    "You have to provide your plugin instance for hooks. "
                    "Do not call bgd::hook::add_hook manually to bypass this – "
                    "it will make the mod unable to properly unload."
                );
                auto res = bgd::hook::add_hook<Func, CallConv>(addr, plugin);
                if (!res) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
            }
            template<typename HookFunc>
            CreateHook(HookFunc addr, bgd::BGDPlugin* plugin) {
                static_assert(
                    !std::is_null_pointer<plugin>::value,
                    "You have to provide your plugin instance for hooks. "
                    "Do not call bgd::hook::add_hook manually to bypass this – "
                    "it will make the mod unable to properly unload."
                );
                // cringe++ wont let me convert addr directly to uintptr_t
                auto addr_ = &addr; 
                auto res = bgd::hook::add_hook<Func, CallConv>(addr_, plugin);
                if (!res) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
            }
            CreateHook(const char* module, uintptr_t addr, bgd::BGDPlugin* plugin) {
                static_assert(
                    !std::is_null_pointer<plugin>::value,
                    "You have to provide your plugin instance for hooks. "
                    "Do not call bgd::hook::add_hook manually to bypass this – "
                    "it will make the mod unable to properly unload."
                );
                HMODULE mod;
                if (m_mods.count(module)) {
                    mod = m_mods[module];
                } else {
                    mod = GetModuleHandleA(module);
                    m_mods[module] = mod;
                }
                if (!mod) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
                auto res = bgd::hook::add_hook<Func, CallConv>(as<uintptr_t>(mod) + addr, plugin);
                if (!res) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
            }
            template<typename HookFunc>
            CreateHook(const char* module, HookFunc addr, bgd::BGDPlugin* plugin) {
                static_assert(
                    !std::is_null_pointer<plugin>::value,
                    "You have to provide your plugin instance for hooks. "
                    "Do not call bgd::hook::add_hook manually to bypass this – "
                    "it will make the mod unable to properly unload."
                );
                HMODULE mod;
                if (m_mods.count(module)) {
                    mod = m_mods[module];
                } else {
                    mod = GetModuleHandleA(module);
                    m_mods[module] = mod;
                }
                if (!mod) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
                // cringe++ wont let me convert addr directly to uintptr_t
                auto addr_ = &addr; 
                auto res = bgd::hook::add_hook<Func, CallConv>(as<uintptr_t>(mod) + as<uintptr_t>(addr_), plugin);
                if (!res) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
            }
            CreateHook(const char* module, const char* symbol, bgd::BGDPlugin* plugin) {
                static_assert(
                    !std::is_null_pointer<plugin>::value,
                    "You have to provide your plugin instance for hooks. "
                    "Do not call bgd::hook::add_hook manually to bypass this – "
                    "it will make the mod unable to properly unload."
                );
                HMODULE mod;
                if (m_mods.count(module)) {
                    mod = m_mods[module];
                } else {
                    mod = GetModuleHandleA(module);
                    m_mods[module] = mod;
                }
                if (!mod) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
                auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
                if (!addr) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find symbol \""_s + symbol + "\" in module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
                auto res = bgd::hook::add_hook<Func, CallConv>(addr, plugin);
                if (!res) {
                    BGDLoader::get()->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook,
                        plugin
                    });
                }
            }
    };
}
