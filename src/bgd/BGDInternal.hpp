#pragma once

#include <BGDUtils.hpp>
#include <BGDPlugin.hpp>

USE_BGD_NAMESPACE();

class BGDInternal {
    private:
        void loadKeybinds();
        
    public:
        void addResourceSearchPaths();
        void setup();
        static bool isFileInSearchPaths(const char*);

        bool loadHooks();

        static BGDInternal* get();
};

class BGDInternalPlugin : public BGDPlugin {
    protected:
        void setup() override;
    
    public:
        static BGDInternalPlugin* get();
};

template<auto Func, typename CallConv = bgd::hook::Optcall>
struct InternalCreateHook {
    private:
        static inline std::unordered_map<std::string, HMODULE> m_mods = {};
        
    public:
        InternalCreateHook(uintptr_t addr) {
            BGDInternalPlugin::get()->throwError(BGDError {
                "creating hook addr",
                "hoo addr",
                kBGDSeverityError,
                kBGDErrorTypeHook
            });
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(addr);
            if (!res) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
        }
        template<typename HookFunc>
        InternalCreateHook(bool, HookFunc addr) {
            // cringe++ wont let me convert addr directly to uintptr_t
            auto addr_ = &addr; 
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(addr_);
            if (!res) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
        }
        InternalCreateHook(const char* module, int addr) {
            BGDInternalPlugin::get()->throwError(BGDError {
                "creating hook mod + addr",
                "hoo mod + addr",
                kBGDSeverityError,
                kBGDErrorTypeHook
            });
            HMODULE mod;
            if (m_mods.count(module)) {
                mod = m_mods[module];
            } else {
                mod = GetModuleHandleA(module);
                m_mods[module] = mod;
            }
            if (!mod) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(as<uintptr_t>(mod) + addr);
            if (!res) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
        }
        template<typename HookFunc>
        InternalCreateHook(const char* module, bool, HookFunc addr) {
            HMODULE mod;
            if (m_mods.count(module)) {
                mod = m_mods[module];
            } else {
                mod = GetModuleHandleA(module);
                m_mods[module] = mod;
            }
            if (!mod) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
            // cringe++ wont let me convert addr directly to uintptr_t
            auto addr_ = &addr; 
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(as<uintptr_t>(mod) + as<uintptr_t>(addr_));
            if (!res) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
        }
        InternalCreateHook(const char* module, const char* symbol) {
            HMODULE mod;
            if (m_mods.count(module)) {
                mod = m_mods[module];
            } else {
                mod = GetModuleHandleA(module);
                m_mods[module] = mod;
            }
            if (!mod) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
            auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
            if (!addr) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find symbol \""_s + symbol + "\" in module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(addr);
            if (!res) {
                BGDInternalPlugin::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook
                });
            }
        }
};
