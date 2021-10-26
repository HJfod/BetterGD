#include <utils/other/ext.hpp>
#include <algorithm>

std::string & bgd::string_to_lower_self(std::string & str) {
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        [](unsigned char c) {
            return std::tolower(c);
        }
    );
    return str;
}

std::string bgd::string_to_lower(std::string const& str) {
    auto ret = str;
    return bgd::string_to_lower_self(ret);
}

std::string & bgd::string_replace_self(
    std::string & str,
    std::string const& orig,
    std::string const& repl
) {
    std::string::size_type n = 0;
    while ( ( n = str.find( orig, n ) ) != std::string::npos ) {
        str.replace( n, orig.size(), repl );
        n += repl.size();
    }
    return str;
}

std::string bgd::string_replace(
    std::string const& str,
    std::string const& orig,
    std::string const& repl
) {
    auto ret = str;
    return bgd::string_replace_self(ret, orig, repl);
}

std::vector<std::string> bgd::string_split(
    std::string const& str,
    std::string const& split
) {
    std::vector<std::string> res;
    if (str.size()) {
        auto s = str;
        size_t pos = 0;
        while ((pos = s.find(split)) != std::string::npos) {
            res.push_back(s.substr(0, pos));
            s.erase(0, pos + split.length());
        }
        if (s.size())
            res.push_back(s);
    }
    return res;
}

std::vector<char> bgd::string_split(std::string const& str) {
    std::vector<char> res;
    for (auto const& s : str) {
        res.push_back(s);
    }
    return res;
}

bool bgd::string_contains(std::string const& str, std::string const& subs) {
    return str.find(subs) != std::string::npos;
}

bool bgd::string_contains(std::string const& str, char c) {
    return str.find(c) != std::string::npos;
}

bool bgd::string_contains_any(std::string const& str, std::vector<std::string> const& subs) {
    for (auto const& sub : subs) {
        if (bgd::string_contains(str, sub))
            return true;
    }
    return false;
}

bool bgd::string_contains_all(std::string const& str, std::vector<std::string> const& subs) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!bgd::string_contains(str, sub))
            found = false;
    }
    return found;
}

bool bgd::string_ends_with(std::string const& value, std::string const& ending) {
    // https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

size_t bgd::string_count(std::string const& str, char countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

std::string & bgd::string_trim_left_self(std::string & str) {
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }
    ));
    return str;
}

std::string & bgd::string_trim_right_self(std::string & str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

std::string & bgd::string_trim_self(std::string & str) {
    return 
        bgd::string_trim_left_self(
        bgd::string_trim_right_self(
            str
        ));
}

std::string bgd::string_trim_left(std::string const& str) {
    auto s2 = str;
    return bgd::string_trim_left_self(s2);
}

std::string bgd::string_trim_right(std::string const& str) {
    auto ret = str;
    return bgd::string_trim_right_self(ret);
}

std::string bgd::string_trim(std::string const& str) {
    auto ret = str;
    return bgd::string_trim_self(ret);
}

std::string & bgd::string_normalize_self(std::string & str) {
    while (string_contains(str, "  "))
        string_replace_self(str, "  ", " ");
    return str;
}

std::string bgd::string_normalize(std::string const& str) {
    auto ret = str;
    return bgd::string_normalize_self(ret);
}

std::string operator"" _s (const char* string, size_t) {
    return std::string(string);
}
