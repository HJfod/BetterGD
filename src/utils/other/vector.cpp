#include <utils/other/ext.hpp>

template<class T>
bool bgd::vector_contains(std::vector<T> const& vec, T elem) {
    return std::find(vec.begin(), vec.end(), elem) != vec.end();
}

template<class T>
bool bgd::vector_contains(std::vector<T> const& vec, std::function<bool(T)> containFunc) {
    for (auto const& item : vec) {
        if (containFunc(item))
            return true;
    }
    return false;
}

template<class T>
void bgd::vector_push(std::vector<T> & vec, std::vector<T> const& subVec) {
    vec.insert(vec.begin(), subVec.begin(), subVec.end());
}

template<class T>
std::string bgd::vector_join(std::vector<T> const& vec, std::string const& sep) {
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

template<class T, class T2>
std::vector<T2> bgd::vector_map(std::vector<T> const& vec, std::function<T2(T)> mapFunc) {
    std::vector<T2, Alloc_T2> res;
    for (auto m : vec) {
        res.push_back(mapFunc(m));
    }
    return res;
}

template<class T>
std::vector<T>& bgd::vector_filter_self(std::vector<T> & vec, std::function<bool(T)> filterFunc) {
    std::vector<T, Alloc_T> res;
    for (auto m : vec) {
        if (filterFunc(m)) {
            res.push_back(m);
        }
    }
    vec = res;
    return vec;
}

template<class T>
std::vector<T> bgd::vector_filter(std::vector<T> const& vec, std::function<bool(T)> filterFunc) {
    std::vector<T, Alloc_T> res;
    for (auto m : vec) {
        if (filterFunc(m)) {
            res.push_back(m);
        }
    }
    return res;
}

template<class T>
T bgd::vector_select(std::vector<T> const& vec, std::function<bool(T)> selectFunc) {
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

template<class T>
std::vector<T> bgd::vector_select_all(std::vector<T> const& vec, std::function<bool(T)> selectFunc) {
    return vector_filter<T>(vec, selectFunc);
}
