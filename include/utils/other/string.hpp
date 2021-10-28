#pragma once

#include <BGDMacros.hpp>
#include <string>
#include <vector>
#include <functional>

namespace bgd {
    #define BGD_COERCE_CHAR_TYPE_TO_STRING(typeName) \
        using typeName = String;                                                \
        if constexpr (std::is_same<String, const char*>::value) {               \
            using typeName = std::string;                                       \
        } else if constexpr (std::is_same<String, const wchar_t*>::value) {     \
            using typeName = std::wstring;                                      \
        }

    template<class String = std::string>
    bool string_ends_with(String const& str, String const& substr) {
        // https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
        if (substr.size() > str.size()) return false;
        return std::equal(substr.rbegin(), substr.rend(), str.rbegin());
    }

    template<class String = std::string>
    String& string_to_lower_self(String& str) {
        std::transform(
            str.begin(), str.end(),
            str.begin(),
            [](auto c) {
                if constexpr (std::is_same<String, std::wstring>::value) {
                    return std::towlower(c);
                } else {
                    return std::tolower(c);
                }
            }
        );
        return str;
    }
    template<class String = std::string>
    String  string_to_lower(String const& str) {
        BGD_COERCE_CHAR_TYPE_TO_STRING(TString);
        TString ret = str;
        return bgd::string_to_lower_self<TString>(ret);
    }

    template<class String = std::string>
    String& string_replace_self(
        String & str,
        String const& orig,
        String const& repl
    ) {
        String::size_type n = 0;
        while ( ( n = str.find( orig, n ) ) != String::npos ) {
            str.replace( n, orig.size(), repl );
            n += repl.size();
        }
        return str;
    }
    template<class String = std::string>
    String  string_replace(
        String const& str,
        String const& orig,
        String const& repl
    ) {
        auto ret = str;
        return bgd::string_replace_self<String>(ret, orig, repl);
    }

    template<class String = std::string>
    std::vector<String> string_split(
        String const& str,
        String const& split
    ) {
        std::vector<String> res;
        if (str.size()) {
            auto s = str;
            size_t pos = 0;
            while ((pos = s.find(split)) != String::npos) {
                res.push_back(s.substr(0, pos));
                s.erase(0, pos + split.length());
            }
            if (s.size())
                res.push_back(s);
        }
        return res;
    }
    template<class String = std::string>
    std::vector<typename String::value_type> string_split(String const& str) {
        std::vector<String::value_type> res;
        for (auto const& s : str) {
            res.push_back(s);
        }
        return res;
    }

    template<class String = std::string>
    bool string_contains(String const& str, String const& subs) {
        return str.find(subs) != String::npos;
    }
    template<class String = std::string>
    bool string_contains(String const& str, typename String::value_type c) {
        return str.find(c) != String::npos;
    }

    template<class String = std::string>
    bool string_contains_any(
        String const& str,
        std::vector<String> const& subs
    ) {
        for (auto const& sub : subs) {
            if (bgd::string_contains<String>(str, sub))
                return true;
        }
        return false;
    }
    template<class String = std::string>
    bool string_contains_all(
        String const& str,
        std::vector<String> const& subs
    ) {
        bool found = true;
        for (auto const& sub : subs) {
            if (!bgd::string_contains<String>(str, sub))
                found = false;
        }
        return found;
    }

    template<class String = std::string>
    size_t string_count(String const& str, typename String::value_type c) {
        size_t res = 0;
        for (auto c : str)
            if (c == countC) res++;
        return res;
    }

    template<class String = std::string>
    String& string_trim_left_self(String & str) {
        str.erase(str.begin(),
            std::find_if(str.begin(), str.end(), [](auto ch) {
                return !std::isspace(ch);
            }
        ));
        return str;
    }
    template<class String = std::string>
    String& string_trim_right_self(String & str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](auto ch) {
            return !std::isspace(ch);
        }).base(), str.end());
        return str;
    }
    template<class String = std::string>
    String& string_trim_self(String & str) {
        return 
            bgd::string_trim_left_self(
            bgd::string_trim_right_self(
                str
            ));
    }

    template<class String = std::string>
    String  string_trim_left(String const& str) {
        auto s2 = str;
        return bgd::string_trim_left_self(s2);
    }
    template<class String = std::string>
    String  string_trim_right(String const& str) {
        auto ret = str;
        return bgd::string_trim_right_self(ret);
    }
    template<class String = std::string>
    String  string_trim(String const& str) {
        auto ret = str;
        return bgd::string_trim_self(ret);
    }

    template<class String = std::string>
    String& string_normalize_self(String & str) {
        while (string_contains<String>(str, "  "))
            string_replace_self(str, "  ", " ");
        return str;
    }
    template<class String = std::string>
    String  string_normalize(String const& str) {
        auto ret = str;
        return bgd::string_normalize_self(ret);
    }
}

std::string operator"" _s (const char* string, size_t);
