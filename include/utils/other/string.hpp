#pragma once

#include <BGDMacros.hpp>
#include <string>
#include <vector>
#include <functional>

namespace bgd {
    /**
     * Convert std::wstring to std::string (UTF-8)
     * @param str String to convert
     * @returns std::string
     */
    BGD_DLL std::string  string_convert(std::wstring const& str);
    /**
     * Convert std::string (UTF-8) to std::wstring
     * @param str String to convert
     * @returns std::wstring
     */
    BGD_DLL std::wstring string_convert(std::string  const& str);

    BGD_DLL bool string_ends_with(std::string  const& str, std::string  const& substr);
    BGD_DLL bool string_ends_with(std::wstring const& str, std::wstring const& substr);

    BGD_DLL std::string & string_to_lower_self(std::string & str);
    BGD_DLL std::wstring& string_to_lower_self(std::wstring& str);

    BGD_DLL std::string  string_to_lower(std::string  const& str);
    BGD_DLL std::wstring string_to_lower(std::wstring const& str);

    BGD_DLL std::string& string_replace_self(
        std::string & str,
        std::string const& orig,
        std::string const& repl
    );
    BGD_DLL std::wstring& string_replace_self(
        std::wstring & str,
        std::wstring const& orig,
        std::wstring const& repl
    );

    BGD_DLL std::string string_replace(
        std::string const& str,
        std::string const& orig,
        std::string const& repl
    );
    BGD_DLL std::wstring string_replace(
        std::wstring const& str,
        std::wstring const& orig,
        std::wstring const& repl
    );

    BGD_DLL std::vector<std::string> string_split(
        std::string const& str,
        std::string const& split
    );
    BGD_DLL std::vector<std::wstring> string_split(
        std::wstring const& str,
        std::wstring const& split
    );

    BGD_DLL std::vector<char>    string_split(std::string  const& str);
    BGD_DLL std::vector<wchar_t> string_split(std::wstring const& str);

    BGD_DLL bool string_contains(std::string  const& str, std::string  const& subs);
    BGD_DLL bool string_contains(std::wstring const& str, std::wstring const& subs);

    BGD_DLL bool string_contains(std::string  const& str, typename char    c);
    BGD_DLL bool string_contains(std::wstring const& str, typename wchar_t c);

    BGD_DLL bool string_contains_any(
        std::string const& str,
        std::vector<std::string> const& subs
    );
    BGD_DLL bool string_contains_any(
        std::wstring const& str,
        std::vector<std::wstring> const& subs
    );

    BGD_DLL bool string_contains_all(
        std::string const& str,
        std::vector<std::string> const& subs
    );
    BGD_DLL bool string_contains_all(
        std::wstring const& str,
        std::vector<std::wstring> const& subs
    );

    BGD_DLL size_t string_count(std::string  const& str, char c);
    BGD_DLL size_t string_count(std::wstring const& str, wchar_t c);

    BGD_DLL std::string & string_trim_left_self(std::string & str);
    BGD_DLL std::wstring& string_trim_left_self(std::wstring& str);
    BGD_DLL std::string & string_trim_right_self(std::string & str);
    BGD_DLL std::wstring& string_trim_right_self(std::wstring& str);
    BGD_DLL std::string & string_trim_self(std::string & str);
    BGD_DLL std::wstring& string_trim_self(std::wstring& str);

    BGD_DLL std::string   string_trim_left(std::string  const& str);
    BGD_DLL std::wstring  string_trim_left(std::wstring const& str);
    BGD_DLL std::string   string_trim_right(std::string  const& str);
    BGD_DLL std::wstring  string_trim_right(std::wstring const& str);
    BGD_DLL std::string   string_trim(std::string  const& str);
    BGD_DLL std::wstring  string_trim(std::wstring const& str);

    BGD_DLL std::string & string_normalize_self(std::string  & str);
    BGD_DLL std::wstring& string_normalize_self(std::wstring & str);
    BGD_DLL std::string   string_normalize(std::string  const& str);
    BGD_DLL std::wstring  string_normalize(std::wstring const& str);

    std::string  operator"" _s  (const char*    string, size_t);
    std::wstring operator"" _ws (const char*    string, size_t);
    std::wstring operator"" _ws (const wchar_t* string, size_t);
}

