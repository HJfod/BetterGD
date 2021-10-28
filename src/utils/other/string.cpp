#include <utils/other/ext.hpp>
#include <algorithm>
#include <cwctype>
#include <Windows.h>
#include <stringapiset.h>

std::string bgd::string_convert(std::wstring const& wstr) {
    int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;;
}

std::wstring bgd::string_convert(std::string const& str) {
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}

bool bgd::string_ends_with(std::string const& str, std::string const& substr) {
    // https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (substr.size() > str.size()) return false;
    return std::equal(substr.rbegin(), substr.rend(), str.rbegin());
}

bool bgd::string_ends_with(std::wstring const& str, std::wstring const& substr) {
    if (substr.size() > str.size()) return false;
    return std::equal(substr.rbegin(), substr.rend(), str.rbegin());
}

std::string& bgd::string_to_lower_self(std::string& str) {
    std::transform(
        str.begin(), str.end(),
        str.begin(),
        [](auto c) {
            return std::tolower(c);
        }
    );
    return str;
}

std::wstring& bgd::string_to_lower_self(std::wstring& str) {
    std::transform(
        str.begin(), str.end(),
        str.begin(),
        [](auto c) {
            return std::towlower(c);
        }
    );
    return str;
}

std::string bgd::string_to_lower(std::string const& str) {
    std::string ret = str;
    return bgd::string_to_lower_self(ret);
}

std::wstring bgd::string_to_lower(std::wstring const& str) {
    std::wstring ret = str;
    return bgd::string_to_lower_self(ret);
}

std::string& bgd::string_replace_self(
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

std::wstring& bgd::string_replace_self(
    std::wstring & str,
    std::wstring const& orig,
    std::wstring const& repl
) {
    std::wstring::size_type n = 0;
    while ( ( n = str.find( orig, n ) ) != std::wstring::npos ) {
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

std::wstring bgd::string_replace(
    std::wstring const& str,
    std::wstring const& orig,
    std::wstring const& repl
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

std::vector<std::wstring> bgd::string_split(
    std::wstring const& str,
    std::wstring const& split
) {
    std::vector<std::wstring> res;
    if (str.size()) {
        auto s = str;
        size_t pos = 0;
        while ((pos = s.find(split)) != std::wstring::npos) {
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

std::vector<wchar_t> bgd::string_split(std::wstring const& str) {
    std::vector<wchar_t> res;
    for (auto const& s : str) {
        res.push_back(s);
    }
    return res;
}

bool bgd::string_contains(std::string const& str, std::string const& subs) {
    return str.find(subs) != std::string::npos;
}

bool bgd::string_contains(std::wstring const& str, std::wstring const& subs) {
    return str.find(subs) != std::wstring::npos;
}

bool bgd::string_contains(std::string const& str, std::string::value_type c) {
    return str.find(c) != std::string::npos;
}

bool bgd::string_contains(std::wstring const& str, std::wstring::value_type c) {
    return str.find(c) != std::wstring::npos;
}

bool bgd::string_contains_any(
    std::string const& str,
    std::vector<std::string> const& subs
) {
    for (auto const& sub : subs) {
        if (bgd::string_contains(str, sub))
            return true;
    }
    return false;
}

bool bgd::string_contains_any(
    std::wstring const& str,
    std::vector<std::wstring> const& subs
) {
    for (auto const& sub : subs) {
        if (bgd::string_contains(str, sub))
            return true;
    }
    return false;
}

bool bgd::string_contains_all(
    std::string const& str,
    std::vector<std::string> const& subs
) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!bgd::string_contains(str, sub))
            found = false;
    }
    return found;
}

bool bgd::string_contains_all(
    std::wstring const& str,
    std::vector<std::wstring> const& subs
) {
    bool found = true;
    for (auto const& sub : subs) {
        if (!bgd::string_contains(str, sub))
            found = false;
    }
    return found;
}

size_t bgd::string_count(std::string const& str, char countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

size_t bgd::string_count(std::wstring const& str, wchar_t countC) {
    size_t res = 0;
    for (auto c : str)
        if (c == countC) res++;
    return res;
}

std::string& bgd::string_trim_left_self(std::string & str) {
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(), [](auto ch) {
            return !std::isspace(ch);
        }
    ));
    return str;
}

std::wstring& bgd::string_trim_left_self(std::wstring & str) {
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(), [](auto ch) {
            return !std::isspace(ch);
        }
    ));
    return str;
}

std::string& bgd::string_trim_right_self(std::string & str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](auto ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

std::wstring& bgd::string_trim_right_self(std::wstring & str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](auto ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}

std::string& bgd::string_trim_self(std::string & str) {
    return 
        bgd::string_trim_left_self(
        bgd::string_trim_right_self(
            str
        ));
}

std::wstring& bgd::string_trim_self(std::wstring & str) {
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

std::wstring bgd::string_trim_left(std::wstring const& str) {
    auto s2 = str;
    return bgd::string_trim_left_self(s2);
}

std::string bgd::string_trim_right(std::string const& str) {
    auto ret = str;
    return bgd::string_trim_right_self(ret);
}

std::wstring bgd::string_trim_right(std::wstring const& str) {
    auto ret = str;
    return bgd::string_trim_right_self(ret);
}

std::string bgd::string_trim(std::string const& str) {
    auto ret = str;
    return bgd::string_trim_self(ret);
}

std::wstring bgd::string_trim(std::wstring const& str) {
    auto ret = str;
    return bgd::string_trim_self(ret);
}

std::string& bgd::string_normalize_self(std::string & str) {
    while (string_contains(str, "  "))
        string_replace_self(str, "  ", " ");
    return str;
}

std::wstring& bgd::string_normalize_self(std::wstring & str) {
    while (string_contains(str, L"  "))
        string_replace_self(str, L"  ", L" ");
    return str;
}

std::string bgd::string_normalize(std::string const& str) {
    auto ret = str;
    return bgd::string_normalize_self(ret);
}

std::wstring bgd::string_normalize(std::wstring const& str) {
    auto ret = str;
    return bgd::string_normalize_self(ret);
}

std::string bgd::operator"" _s (const char* string, size_t) {
    return string;
}

std::wstring bgd::operator"" _ws (const char* string, size_t) {
    return string_convert(string);
}

std::wstring bgd::operator"" _ws (const wchar_t* string, size_t) {
    return string;
}
