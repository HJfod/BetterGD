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

    template<class T, class R>
    std::vector<R> map_select_all(
        std::unordered_map<T, R> map,
        std::function<bool(R)> selectFunc
    ) {
        std::vector<R> res;
        for (auto const& [_, r] : map) {
            if (selectFunc(r)) {
                res.push_back(r);
            }
        }
        return res;
    }

    template<class T, class R>
    std::vector<R> map_values(std::unordered_map<T, R> map) {
        std::vector<R> res;
        for (auto const& [_, r] : map) {
            res.push_back(r);
        }
        return res;
    }

    template<class T, class R>
    std::vector<T> map_keys(std::unordered_map<T, R> map) {
        std::vector<T> res;
        for (auto const& [t, _] : map) {
            res.push_back(t);
        }
        return res;
    }
}
