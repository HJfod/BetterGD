#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

using arg_name_type  = std::string;
using arg_value_type = std::string;

class ArgParser;

class ParsedArgs {
    protected:
        std::unordered_map<arg_name_type, arg_value_type> m_mFlags;
        std::vector<arg_name_type> m_vArgs;

        friend class ArgParser;
    
    public:
        bool hasFlag(arg_name_type const& name) const;
        arg_value_type getFlagValue(
            arg_name_type const& name,
            arg_value_type const& default = arg_value_type()
        ) const;
        std::vector<arg_name_type> getFlags() const;

        bool hasArg(size_t index) const;
        arg_value_type at(size_t index) const;
        std::vector<arg_name_type> getArgs() const;
};

class ArgParser {
    public:
        Result<ParsedArgs> parse(std::string const& args);
};
