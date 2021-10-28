#include <utils/other/ext.hpp>
#include <algorithm>
#include <cwctype>

std::string operator"" _s (const char* string, size_t) {
    return std::string(string);
}
