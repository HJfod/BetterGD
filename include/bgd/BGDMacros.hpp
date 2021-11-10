#pragma once

// Set dllexport/dllimport to BGD classes & functions
#ifdef _EXPORTING
   #define BGD_DLL    __declspec(dllexport)
#else
   #define BGD_DLL    __declspec(dllimport)
#endif

// Macros for making default functions for plugins
#define BGD_LOAD($pluginName) \
    extern "C" { __declspec(dllexport) bgd::BGDPlugin* __stdcall bgd_load() { return $pluginName::get(); } };

#define BGD_PLUGIN_GET($pluginName)             \
    public:                                     \
    static inline $pluginName* get() {          \
        static auto g_plugin = new $pluginName; \
        return g_plugin;                        \
    }

// Win32
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define BGD_WIN32(...) __VA_ARGS__
    #define BGD_IS_WIN32
    #define BGD_IS_DESKTOP
#else
    #define BGD_WIN32(...)
#endif

// TODO: Add more platforms (Mac, Android?)

#ifdef BGD_IS_DESKTOP

    // comment in/out if platform console should be enabled/disabled
    #define BGD_PLATFORM_CONSOLE(...) __VA_ARGS__
    #define BGD_SHOW_PLATFORM_CONSOLE

#endif
