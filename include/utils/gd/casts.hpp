#pragma once

#include "include_gd.hpp"

// alias for reinterpret_cast
template <typename T, typename R>
T as(R const v);

// cast from any type to an enum
template<typename E, typename T>
constexpr E enum_cast(T type);

std::string operator"" _s (const char* string, size_t);
