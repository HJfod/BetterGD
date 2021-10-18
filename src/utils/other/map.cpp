#include <utils/other/map.hpp>

template<class T, class R>
bool bgd::map_contains(
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
R bgd::map_select(
    std::unordered_map<T, R> map,
    std::function<bool(R)> containFunc
) {
    for (auto const& [_, r] : map) {
        if (containFunc(r))
            return r;
    }
    if (std::is_pointer<R>::value) {
        return nullptr;
    }
    return R();
}
