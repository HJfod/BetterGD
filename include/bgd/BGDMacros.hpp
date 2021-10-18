#pragma once

#define BGD_DLL __declspec(dllexport)

#define BGD_LOAD($pluginName) \
    extern "C" { __declspec(dllexport) bgd::BGDPlugin* __stdcall bgd_load() { return $pluginName::get(); } };

#define BGD_PLUGIN_GET($pluginName)             \
    public:                                     \
    static inline $pluginName* get() {          \
        static auto g_plugin = new $pluginName; \
        return g_plugin;                        \
    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define BGD_WIN32(...) __VA_ARGS__
    #define BGD_IS_WIN32
#else
    #define BGD_WIN32(...)
#endif
