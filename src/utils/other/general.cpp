#include <utils/other/general.hpp>

constexpr unsigned int hash(const char* str, int h) {
    return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
}

constexpr size_t operator"" _h (const char* txt, size_t) {
    return hash(txt);
}

template<typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return v < lo ? lo : hi < v ? hi : v;
}

template<typename T, typename U>
constexpr size_t offsetOf(U T::*member) {
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

