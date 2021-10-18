#pragma once

#include <string>
#include <vector>
#include <functional>

namespace bgd {
    template<class T, class R>
    bool map_contains(
        std::unordered_map<T, R> map,
        std::function<bool(R)> containFunc
    );

    template<class T, class R>
    R map_select(
        std::unordered_map<T, R> map,
        std::function<bool(R)> selectFunc
    );
}
