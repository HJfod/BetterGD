#pragma once

#include "matdash.hpp"
#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>

namespace bgd {
    #define CREATE_HOOK(func, abs_addr, plugin) \
        static bgd::CreateHook<&func> __STR_CAT__($,__LINE__) (abs_addr, plugin);

    #define CREATE_HOOK_GD(func, addr, plugin) \
        static bgd::CreateHook<&func> __STR_CAT__($,__LINE__) (gd::base + addr, plugin);

    template<auto Func, typename CallConv = bgd::hook::Optcall>
    struct CreateHook {
        private:
            static inline std::unordered_map<std::string, HMODULE> m_mods = {};
            
        public:
            CreateHook(uintptr_t addr, bgd::BGDPlugin* plugin) {
                auto res = plugin->addHook<Func, CallConv>(addr);
                if (!res) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
            }
            template<typename HookFunc>
            CreateHook(HookFunc addr, bgd::BGDPlugin* plugin) {
                // cringe++ wont let me convert addr directly to uintptr_t
                auto addr_ = &addr; 
                auto res = plugin->addHook<Func, CallConv>(addr_);
                if (!res) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
            }
            CreateHook(const char* module, uintptr_t addr, bgd::BGDPlugin* plugin) {
                HMODULE mod;
                if (m_mods.count(module)) {
                    mod = m_mods[module];
                } else {
                    mod = GetModuleHandleA(module);
                    m_mods[module] = mod;
                }
                if (!mod) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
                auto res = plugin->addHook<Func, CallConv>(as<uintptr_t>(mod) + addr);
                if (!res) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
            }
            template<typename HookFunc>
            CreateHook(const char* module, HookFunc addr, bgd::BGDPlugin* plugin) {
                HMODULE mod;
                if (m_mods.count(module)) {
                    mod = m_mods[module];
                } else {
                    mod = GetModuleHandleA(module);
                    m_mods[module] = mod;
                }
                if (!mod) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
                // cringe++ wont let me convert addr directly to uintptr_t
                auto addr_ = &addr; 
                auto res = plugin->addHook<Func, CallConv>(as<uintptr_t>(mod) + as<uintptr_t>(addr_));
                if (!res) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
            }
            CreateHook(const char* module, const char* symbol, bgd::BGDPlugin* plugin) {
                HMODULE mod;
                if (m_mods.count(module)) {
                    mod = m_mods[module];
                } else {
                    mod = GetModuleHandleA(module);
                    m_mods[module] = mod;
                }
                if (!mod) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
                auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
                if (!addr) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        "Unable to find symbol \""_s + symbol + "\" in module \""_s + module + "\"",
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
                auto res = plugin->addHook<Func, CallConv>(addr);
                if (!res) {
                    plugin->throwError(BGDError {
                        "Error Creating Hook",
                        res.error(),
                        kBGDSeverityError,
                        kBGDErrorTypeHook
                    });
                }
            }
    };
}
