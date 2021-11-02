#pragma once

#include <BGDUtils.hpp>
#include <BGDPlugin.hpp>

USE_BGD_NAMESPACE();

class BGDInternal {
    private:
    public:
        void addResourceSearchPaths();
        void setup();
        Result<> addHook(void* addr, void* detour, BGDPlugin* owner);
        static bool isFileInSearchPaths(const char*);

        static BGDInternal* get();
};

class BGDInteralPlugin : public BGDPlugin {
    protected:
        void setup() override;
    
    public:
        static BGDInteralPlugin* get();
};

template<auto Func, typename CallConv = bgd::hook::Optcall>
struct InternalCreateHook {
    private:
        static inline std::unordered_map<std::string, HMODULE> m_mods = {};
        
    public:
        InternalCreateHook(uintptr_t addr) {
            auto res = bgd::hook::add_hook<Func, CallConv>(
                addr,
                BGDInteralPlugin::get()
            );
            if (!res) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
        }
        template<typename HookFunc>
        InternalCreateHook(HookFunc addr) {
            // cringe++ wont let me convert addr directly to uintptr_t
            auto addr_ = &addr; 
            auto res = bgd::hook::add_hook<Func, CallConv>(
                addr_,
                BGDInteralPlugin::get()
            );
            if (!res) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
        }
        InternalCreateHook(const char* module, uintptr_t addr) {
            HMODULE mod;
            if (m_mods.count(module)) {
                mod = m_mods[module];
            } else {
                mod = GetModuleHandleA(module);
                m_mods[module] = mod;
            }
            if (!mod) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
            auto res = bgd::hook::add_hook<Func, CallConv>(
                as<uintptr_t>(mod) + addr,
                BGDInteralPlugin::get()
            );
            if (!res) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
        }
        template<typename HookFunc>
        InternalCreateHook(const char* module, HookFunc addr) {
            HMODULE mod;
            if (m_mods.count(module)) {
                mod = m_mods[module];
            } else {
                mod = GetModuleHandleA(module);
                m_mods[module] = mod;
            }
            if (!mod) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
            // cringe++ wont let me convert addr directly to uintptr_t
            auto addr_ = &addr; 
            auto res = bgd::hook::add_hook<Func, CallConv>(
                as<uintptr_t>(mod) + as<uintptr_t>(addr_),
                BGDInteralPlugin::get()
            );
            if (!res) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
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
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
            auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
            if (!addr) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    "Unable to find symbol \""_s + symbol + "\" in module \""_s + module + "\"",
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
            auto res = bgd::hook::add_hook<Func, CallConv>(
                addr,
                BGDInteralPlugin::get()
            );
            if (!res) {
                BGDLoader::get()->throwError(BGDError {
                    "Error Creating Hook",
                    res.error(),
                    kBGDSeverityError,
                    kBGDErrorTypeHook,
                    BGDInteralPlugin::get()
                });
            }
        }
};
