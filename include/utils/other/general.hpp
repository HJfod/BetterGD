#pragma once

namespace bgd {
    constexpr unsigned int hash(const char* str, int h = 0) {
        return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
    }

    constexpr size_t operator"" _h (const char* txt, size_t) {
        return bgd::hash(txt);
    }

    template<typename T>
    constexpr const T& clamp(const T& value, const T& minValue, const T& maxValue) {
        return value < minValue ? minValue : maxValue < value ? maxValue : value;
    }

    template<typename T, typename U>
    constexpr size_t offsetOf(U T::*member) {
        return (char*)&((T*)nullptr->*member) - (char*)nullptr;
    }
}
