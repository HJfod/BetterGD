#pragma once

#include <BGDMacros.hpp>
#include <string>
#include <vector>
#include <functional>

namespace bgd {
    template<class T, class R>
    bool map_contains(
        std::unordered_map<T, R> map,
        std::function<bool(R)> containFunc
    ) {
        for (auto const& [_, r] : map) {
            if (containFunc(r))
                return true;
        }
        return false;
    }

    template<class T, class R>
    R map_select(
        std::unordered_map<T, R> map,
        std::function<bool(R)> selectFunc
    ) {
        for (auto const& [_, r] : map) {
            if (selectFunc(r))
                return r;
        }
        if (std::is_pointer<R>::value) {
            return nullptr;
        }
        return R();
    }
}
