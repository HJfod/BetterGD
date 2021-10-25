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

    enum BGDErrorType {
        kBGDErrorTypeGeneral,       // general error
        kBGDErrorTypeUnknown,       // unknown error
        kBGDErrorTypeInit,          // initialization / loading error
        kBGDErrorTypeDeinit,        // deinitialization / unloading error
        kBGDErrorTypeHook,          // hooking error
        kBGDErrorTypeVersion,       // version error
        kBGDErrorTypeMissingData,   // missing required files
    };

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
