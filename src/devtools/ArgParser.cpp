#include "ArgParser.hpp"

bool ParsedArgs::hasFlag(arg_name_type const& name) const {
    return this->m_mFlags.count(name);
}

arg_value_type ParsedArgs::getFlagValue(
    arg_name_type const& name,
    arg_value_type const& default
) const {
    if (!this->m_mFlags.count(name)) {
        return default;
    }
    return this->m_mFlags.at(name);
}

std::vector<arg_name_type> ParsedArgs::getFlags() const {
    return map_keys(this->m_mFlags);
}

bool ParsedArgs::hasArg(size_t index) const {
    return index < this->m_vArgs.size();
}

arg_value_type ParsedArgs::at(size_t index) const {
    if (index < this->m_vArgs.size()) {
        return this->m_vArgs[index];
    }
    return arg_value_type();
}

std::vector<arg_name_type> ParsedArgs::getArgs() const {
    return this->m_vArgs;
}

Result<ParsedArgs> ArgParser::parse(std::string const& msg_raw) {
    if (!msg_raw.size()) {
        return Err<>("String is empty");
    }
    auto msg = msg_raw + " ";
    auto& args = ParsedArgs();
    std::string collect = "";
    std::string flag_name = "";
    char last_char = 0;
    bool is_flag   = false;
    bool in_quotes = false;
    bool escape    = false;
    for (auto const& c : msg) {
        switch (c) {
            case '-':
                // if we're already collecting characters,
                // then checking stuff here would mess with
                // args like `go-to`
                if (!collect.size()) {
                    is_flag = true;
                }
                escape = false;
                break;
            
            case '\\':
                if (escape) {
                    collect += "\\";
                    escape = false;
                } else {
                    escape = true;
                }
                break;

            default:
                escape = false;
                collect += c;
                break;
            
            case '"':
                if (!escape) {
                    in_quotes = !in_quotes;
                    if (in_quotes) {
                        break;
                    }
                }
            
            case ' ':
                escape = false;
                if (in_quotes) {
                    collect += c;
                } else {
                    if (collect.size()) {
                        if (is_flag) {
                            if (flag_name.size()) {
                                args.m_mFlags[flag_name] = "";
                            }
                            flag_name = collect;
                        } else {
                            if (flag_name.size()) {
                                args.m_mFlags[flag_name] = collect;
                            } else {
                                args.m_vArgs.push_back(collect);
                            }
                            flag_name = "";
                        }
                        collect = "";
                        is_flag = false;
                    }
                }
                break;
        }
    }
    if (flag_name.size()) {
        args.m_mFlags[flag_name] = "";
    }
    return Ok<ParsedArgs>(args);
}
