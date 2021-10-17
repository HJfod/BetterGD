#pragma once

constexpr unsigned int hash(const char* str, int h = 0);

constexpr size_t operator"" _h (const char* txt, size_t);

template<typename T>
constexpr const T& clamp(const T& value, const T& minValue, const T& maxValue);

template<typename T, typename U>
constexpr size_t offsetOf(U T::*member);
