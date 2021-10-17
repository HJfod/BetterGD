#pragma once

#include <string>
#include <vector>
#include <functional>

namespace bgd {
    std::string&    string_lower_self(std::string & str);
    std::string     string_lower(std::string const& str);

    std::string&    string_replace_self(
        std::string & str,
        std::string const& orig,
        std::string const& repl
    );
    std::string     string_replace(
        std::string const& str,
        std::string const& orig,
        std::string const& repl
    );

    std::vector<std::string> string_split(
        std::string const& str,
        std::string const& split
    );
    std::vector<char> string_split(std::string const& str);

    bool string_contains(std::string const& str, std::string const& subs);
    bool string_contains(std::string const& str, char c);
    bool string_contains(std::string const& str, std::vector<std::string> const& subs);

    size_t          string_count(std::string const& str, char c);

    std::string& string_trim_left_self(std::string & str);
    std::string& string_trim_right_self(std::string & str);
    std::string& string_trim_self(std::string & str);

    std::string  string_trim_left(std::string const& str);
    std::string  string_trim_right(std::string const& str);
    std::string  string_trim(std::string const& str);

    std::string& string_normalize_self(std::string & str);
    std::string  string_normalize(std::string const& str);
}
