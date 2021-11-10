#pragma once

#include <string>

namespace bgd {
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

    static std::string BGDSeverityToString(BGDSeverity severity) {
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

    static std::string BGDErrorTypeToString(BGDErrorType type) {
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
}
