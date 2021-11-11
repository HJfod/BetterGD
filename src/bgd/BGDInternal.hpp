#pragma once

#include <BGDUtils.hpp>
#include <BGDPlugin.hpp>
#include <BGDLog.hpp>

USE_BGD_NAMESPACE();

inline const char* getNodeName(CCObject* node) {
    return typeid(*node).name() + 6;
}

class BGDInternal {
    protected:
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
    
        BGDInternalPlugin();

    public:
        static BGDInternalPlugin* get();
};

template<auto Func, typename CallConv = bgd::hook::Optcall>
struct InternalCreateHook {
    private:
        static inline std::unordered_map<std::string, HMODULE> m_mods = {};
        
    public:
        InternalCreateHook(uintptr_t addr) {
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(addr);
            if (!res) {
                BGDInternalPlugin::get()->throwError(
                    res.error(),
                    kBGDSeverityCritical
                );
            }
        }
        template<typename HookFunc>
        InternalCreateHook(bool, HookFunc addr) {
            // cringe++ wont let me convert addr directly to uintptr_t
            auto addr_ = &addr; 
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(addr_);
            if (!res) {
                BGDInternalPlugin::get()->throwError(
                    res.error(),
                    kBGDSeverityCritical
                );
            }
        }
        InternalCreateHook(const char* module, int addr) {
            HMODULE mod;
            if (m_mods.count(module)) {
                mod = m_mods[module];
            } else {
                mod = GetModuleHandleA(module);
                m_mods[module] = mod;
            }
            if (!mod) {
                BGDInternalPlugin::get()->throwError(
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityCritical
                );
            }
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(as<uintptr_t>(mod) + addr);
            if (!res) {
                BGDInternalPlugin::get()->throwError(
                    res.error(),
                    kBGDSeverityCritical
                );
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
                BGDInternalPlugin::get()->throwError(
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityCritical
                );
            }
            // cringe++ wont let me convert addr directly to uintptr_t
            auto addr_ = &addr; 
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(as<uintptr_t>(mod) + as<uintptr_t>(addr_));
            if (!res) {
                BGDInternalPlugin::get()->throwError(
                    res.error(),
                    kBGDSeverityCritical
                );
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
                BGDInternalPlugin::get()->throwError(
                    "Unable to find module \""_s + module + "\"",
                    kBGDSeverityCritical
                );
            }
            auto addr = reinterpret_cast<uintptr_t>(GetProcAddress(mod, symbol));
            if (!addr) {
                BGDInternalPlugin::get()->throwError(
                    "Unable to find symbol \""_s + symbol + "\" in module \""_s + module + "\"",
                    kBGDSeverityCritical
                );
            }
            auto res = BGDInternalPlugin::get()->addHook<Func, CallConv>(addr);
            if (!res) {
                BGDInternalPlugin::get()->throwError(
                    res.error(),
                    kBGDSeverityCritical
                );
            }
        }
};
