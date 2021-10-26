#pragma once

#include <BGDMacros.hpp>
#include <string>
#include <vector>
#include <functional>

namespace bgd {
    BGD_DLL bool string_ends_with(std::string const& str, std::string const& substr);

    BGD_DLL std::string& string_to_lower_self(std::string & str);
    BGD_DLL std::string  string_to_lower(std::string const& str);

    BGD_DLL std::string& string_replace_self(
        std::string & str,
        std::string const& orig,
        std::string const& repl
    );
    BGD_DLL std::string  string_replace(
        std::string const& str,
        std::string const& orig,
        std::string const& repl
    );

    BGD_DLL std::vector<std::string> string_split(
        std::string const& str,
        std::string const& split
    );
    BGD_DLL std::vector<char> string_split(std::string const& str);

    BGD_DLL bool string_contains(std::string const& str, std::string const& subs);
    BGD_DLL bool string_contains(std::string const& str, char c);

    BGD_DLL bool string_contains_any(
        std::string const& str,
        std::vector<std::string> const& subs
    );
    BGD_DLL bool string_contains_all(
        std::string const& str,
        std::vector<std::string> const& subs
    );

    BGD_DLL size_t string_count(std::string const& str, char c);

    BGD_DLL std::string& string_trim_left_self(std::string & str);
    BGD_DLL std::string& string_trim_right_self(std::string & str);
    BGD_DLL std::string& string_trim_self(std::string & str);

    BGD_DLL std::string  string_trim_left(std::string const& str);
    BGD_DLL std::string  string_trim_right(std::string const& str);
    BGD_DLL std::string  string_trim(std::string const& str);

    BGD_DLL std::string& string_normalize_self(std::string & str);
    BGD_DLL std::string  string_normalize(std::string const& str);
}

std::string operator"" _s (const char* string, size_t);
