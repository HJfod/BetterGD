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
    };
}
