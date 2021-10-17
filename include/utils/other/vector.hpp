#pragma once

#include <string>
#include <vector>
#include <functional>

namespace bgd {
    template<class T>
    bool vector_contains(std::vector<T> const& vec, T elem);

    template<class T>
    bool vector_contains(std::vector<T> const& vec, std::function<bool(T)> elem);

    template<class T>
    void vector_push(std::vector<T> & vec, std::vector<T> const& sub);

    template<class T>
    std::string vector_join(std::vector<T> const& vec, std::string const& sep);

    template<class T, class T2>
    std::vector<T2> vector_map(std::vector<T> & vec, std::function<T2(T)> func);

    template<class T>
    std::vector<T> vector_filter(std::vector<T> & vec, std::function<bool(T)> func);
}
