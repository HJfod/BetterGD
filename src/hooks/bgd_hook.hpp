#pragma once

#include <BetterGD.hpp>

USE_BGD_NAMESPACE();

#define CREATE_HOOK(func, addr) \
    static CreateHook<&func> __STR_CAT__($,__LINE__) (gd::base + addr);

template<auto Func, typename CallConv = matdash::Optcall>
struct CreateHook {
    CreateHook(uintptr_t addr) {
        matdash::add_hook<Func, CallConv>(addr);
    }
};
