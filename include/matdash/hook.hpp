#pragma once

#include "matdash.hpp"
#include <BGDPlugin.hpp>
#include <BGDLoader.hpp>

namespace bgd {
    #define CREATE_HOOK(func, abs_addr) \
        static bgd::CreateHook<&func> __STR_CAT__($,__LINE__) (abs_addr);

    #define CREATE_HOOK_GD(func, addr) \
        static bgd::CreateHook<&func> __STR_CAT__($,__LINE__) (gd::base + addr);

    template<auto Func, typename CallConv = matdash::Optcall>
    struct CreateHook {
        CreateHook(uintptr_t addr, bgd::BGDPlugin* plugin = nullptr) {
            auto res = matdash::add_hook<Func, CallConv>(addr);
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
        CreateHook(const char* module, const char* symbol, bgd::BGDPlugin* plugin = nullptr) {
            static std::unordered_map<std::string, HMODULE> mods = {};
            HMODULE mod;
            if (mods.count(module)) {
                mod = mods[module];
            } else {
                mod = GetModuleHandleA(module);
                mods[module] = mod;
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
            auto res = matdash::add_hook<Func, CallConv>(addr);
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
