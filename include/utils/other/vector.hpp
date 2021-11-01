#pragma once

#include <BGDMacros.hpp>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

namespace bgd {
    /**
     * Check if a vector contains an element by value.
     * @param vec The vector to check.
     * @param elem The element to find.
     * @returns True if element is in `vec`, false if not.
     */
    template<class T>
    bool vector_contains(std::vector<T> const& vec, T elem) {
        return std::find(vec.begin(), vec.end(), elem) != vec.end();
    }

    /** 
     * Check if a vector contains an element via a function.
     * @param vec The vector to check.
     * @param containFunc A function that returns bool if the
     * element parameter is what is looked for.
     * @returns True if an element matching `containFunc` is
     * in `vec`, false if not.
     */
    template<class T>
    bool vector_contains(std::vector<T> const& vec, std::function<bool(T)> containFunc) {
        for (auto const& item : vec) {
            if (containFunc(item))
                return true;
        }
        return false;
    }

    /** 
     * Add a vector to the end of another vector.
     * @param vec The vector to add to.
     * @param subVec The vector to add.
     */
    template<class T>
    void vector_push(std::vector<T> & vec, std::vector<T> const& subVec) {
        vec.insert(vec.begin(), subVec.begin(), subVec.end());
    }

    /** 
     * Turn a vector into a string. T must be either a string,
     * or convertible via `std::to_string`.
     * @param vec The vector to add to.
     * @param sep Separator string.
     * @returns Joined string.
     */
    template<class T>
    std::string vector_join(std::vector<T> const& vec, std::string const& sep) {
        std::string res = "";

        for (auto p : vec) {
            if (!std::is_same<T, std::string>::value) {
                res += std::to_string(p) + sep;
            } else {
                res += p + sep;
            }
        }
        
        res = res.substr(0, res.length() - sep.length());

        return res;
    }

    /** 
     * Map a vector of items type T to a new vector of items
     * type T2.
     * @param vec The vector to map.
     * @param mapFunc Function that converts an element from T to T2.
     * @returns Mapped vector.
     */
    template<class T, class T2>
    std::vector<T2> vector_map(std::vector<T> const& vec, std::function<T2(T)> mapFunc) {
        std::vector<T2> res;
        for (auto m : vec) {
            res.push_back(mapFunc(m));
        }
        return res;
    }

    /** 
     * Remove items from a vector that don't match the filter
     * predicate.
     * @param vec The vector to filter.
     * @param filterFunc Predicate function that returns true
     * if element should be kept in vector.
     * @returns Filtered vector.
     */
    template<class T>
    std::vector<T>& vector_filter_self(std::vector<T> & vec, std::function<bool(T)> filterFunc) {
        std::vector<T> res;
        for (auto m : vec) {
            if (filterFunc(m)) {
                res.push_back(m);
            }
        }
        vec = res;
        return vec;
    }

    /** 
     * Return a copy of the vector that has items not
     * matching `filterFunc` removed.
     * @param vec The vector to filter.
     * @param filterFunc Predicate function that returns true
     * if element should be kept in vector.
     * @returns Filtered vector.
     */
    template<class T>
    std::vector<T> vector_filter(std::vector<T> const& vec, std::function<bool(T)> filterFunc) {
        std::vector<T> res;
        for (auto m : vec) {
            if (filterFunc(m)) {
                res.push_back(m);
            }
        }
        return res;
    }

    /** 
     * See if an item in the vector matching the predicate
     * `selectFunc` exists, and return it if it does.
     * @param vec The vector to select from.
     * @param selectFunc Predicate function to see if a
     * given item is what is looked for.
     * @returns Found item, or the default of T if it was
     * not found. If T is pointer type and item was not
     * found, the return type is nullptr.
     */
    template<class T>
    T vector_select(std::vector<T> const& vec, std::function<bool(T)> selectFunc) {
        for (auto const& v : vec) {
            if (selectFunc(v)) {
                return v;
            }
        }
        if (std::is_pointer<T>::value) {
            return nullptr;
        }
        return T();
    }

    /** 
     * Alias for `vector_filter`.
     * @param vec The vector to filter.
     * @param selectFunc Predicate function that returns true
     * if element should be kept in vector.
     * @returns Filtered vector.
     */
    template<class T>
    std::vector<T> vector_select_all(std::vector<T> const& vec, std::function<bool(T)> selectFunc) {
        return vector_filter<T>(vec, selectFunc);
    }

    template<class T>
    std::vector<T> & vector_erase(std::vector<T> & vec, T element) {
        vec.erase(std::remove(vec.begin(), vec.end(), element), vec.end());
        return vec;
    }

    template<class T>
    std::vector<T> & vector_erase(std::vector<T> & vec, std::function<bool(T)> eraseFunc) {
        vec.erase(std::remove_if(vec.begin(), vec.end(), eraseFunc), vec.end());
        return vec;
    }

    template<class R, class T>
    R vector_reduce(std::vector<T> const& vec, std::function<void(R&, T)> reduceFunc) {
        R res;
        for (auto const& item : vec) {
            reduceFunc(res, item);
        }
        return res;
    }
}
