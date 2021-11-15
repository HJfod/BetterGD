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
        arg_value_type at(size_t index, arg_value_type const& default = arg_value_type()) const;
        std::vector<arg_name_type> getArgs() const;
};

class ArgParser {
    public:
        Result<ParsedArgs> parse(std::string const& args);
};

#define SWITCH_ARGS_BASE \
    arg_switch_level++; \
    switch (hash(args.at(arg_switch_level).c_str()))

#define SWITCH_ARGS \
    int arg_switch_level = 0; \
    switch (hash(args.at(0).c_str()))

#define HANDLER(x) break; case hash(x):

#define SWITCH_SUB(arg) \
    HANDLER(arg) SWITCH_ARGS_BASE

#define DEFAULT_HANDLER break; default:

#define UNKNOWN_HANDLER()                                   \
    DEFAULT_HANDLER {                                       \
        if (args.hasArg(arg_switch_level)) {                \
            BGDInternalPlugin::get()->log()                 \
                << kBGDSeverityError << kBGDLogTypeError    \
                << "Unknown Command: \""                    \
                << args.at(arg_switch_level) << "\""        \
                << bgd::endl;                               \
        } else {                                            \
            BGDInternalPlugin::get()->log()                 \
                << kBGDSeverityError << kBGDLogTypeError    \
                << "\"" << args.at(arg_switch_level - 1) << "\""\
                << " has not default handler"               \
                << bgd::endl;                               \
        }                                                   \
    }
