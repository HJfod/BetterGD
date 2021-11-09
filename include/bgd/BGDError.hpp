#pragma once

#include <string>

namespace bgd {
    class BGDPlugin;

    enum BGDSeverity {
        kBGDSeverityDebug,      // no need to even pay attention
        kBGDSeverityInfo,       // fyi :)
        kBGDSeverityNotice,     // hmm
        kBGDSeverityWarning,    // oh shit
        kBGDSeverityError,      // oh fuck
        kBGDSeverityCritical,   // oh god oh fuck
        kBGDSeverityAlert,      // panic
        kBGDSeverityEmergency,  // !!!!!!!!!!!!!
    };

    constexpr const char* BGDSeverityToString(BGDSeverity severity) {
        switch (severity) {
            case kBGDSeverityDebug:     return "Debug";
            case kBGDSeverityInfo:      return "Info";
            case kBGDSeverityNotice:    return "Notice";
            case kBGDSeverityWarning:   return "Warning";
            case kBGDSeverityError:     return "Error";
            case kBGDSeverityCritical:  return "Critical";
            case kBGDSeverityAlert:     return "Alert";
            case kBGDSeverityEmergency: return "Emergency";
            default:                    return "Unknown";
        }
    }

    enum BGDErrorType {
        kBGDErrorTypeGeneral,       // general error
        kBGDErrorTypeUnknown,       // unknown error
        kBGDErrorTypeInit,          // initialization / loading error
        kBGDErrorTypeDeinit,        // deinitialization / unloading error
        kBGDErrorTypeHook,          // hooking error
        kBGDErrorTypeVersion,       // version error
        kBGDErrorTypeMissingData,   // missing required files
    };

    constexpr const char* BGDErrorTypeToString(BGDErrorType type) {
        switch (type) {
            case kBGDErrorTypeGeneral:      return "General";
            case kBGDErrorTypeInit:         return "Initialization";
            case kBGDErrorTypeDeinit:       return "Deinitialization";
            case kBGDErrorTypeHook:         return "Hooking";
            case kBGDErrorTypeVersion:      return "Version";
            case kBGDErrorTypeMissingData:  return "Missing Data";
            case kBGDErrorTypeUnknown:default:return "Unknown";
        }
    }

    struct BGDError {
        std::string_view info;          // title / short description
        std::string_view description;   // full description
        BGDSeverity severity;           // severity
        BGDErrorType type;              // type
        BGDPlugin* plugin = nullptr;    // source plugin

        BGDError() = delete;
        BGDError(
            std::string_view info,
            std::string_view description,
            BGDSeverity severity,
            BGDErrorType type
        ) : info(info), description(description),
            severity(severity), type(type) {}
        BGDError(
            std::string_view info,
            std::string_view description,
            BGDSeverity severity,
            BGDErrorType type,
            BGDPlugin* plugin
        ) : info(info), description(description),
            severity(severity), type(type), plugin(plugin) {}
    };
}
