#pragma once

#include <inttypes.h>

namespace bgd {
    // alias for reinterpret_cast
    template <typename T, typename R>
    static constexpr T as(R const v) {
        return reinterpret_cast<T>(v);
    }

    // cast from literally anything to
    // literally anything else (as long as
    // their sizes match)
    template<typename R, typename T>
    static constexpr R union_cast(T v) {
        static_assert(sizeof T == sizeof R, "union_cast: R and T don't match in size!");
        union {
            R r;
            T t;
        } x;
        x.t = v;
        return x.r;
    }

    namespace cast {
        // cast from any (fitting) type to an enum
        template<typename E, typename T>
        static constexpr E enum_cast(T type) {
            return static_cast<E>(reinterpret_cast<int>(type));
        }

        // cast from any type to a bool
        // without compiler warnings
        template <typename T>
        static constexpr bool bool_cast(T const v) {
            return static_cast<bool>(reinterpret_cast<int>(v));
        }

        // cast object to something else, if its
        // vtable offset matches `vtable`
        template<typename T, typename R = T>
        static constexpr R vtable_cast(T obj, uintptr_t vtable) {
            if (obj && (*as<uintptr_t*>(obj) - base == vtable))
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
}
