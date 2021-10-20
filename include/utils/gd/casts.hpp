#pragma once

#include "include_gd.hpp"

namespace bgd::cast {
    // cast object to something else, if its
    // vtable offset matches `vtable`
    template<typename T, typename R = T>
    static constexpr R gd_vtable_cast(T obj, uintptr_t vtable) {
        if (obj && (*as<uintptr_t*>(obj) - gd::base == vtable))
            return as<R>(obj);
        return nullptr;
    }

    // offset a class pointer by `offset`
    // amount
    template<typename T, typename R>
    static constexpr T offset_cast(R const v, uintptr_t offset) {
        return reinterpret_cast<T>(reinterpret_cast<uintptr_t>(v) + offset);
    }
}
