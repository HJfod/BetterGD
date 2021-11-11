#pragma once

#include "BGDMacros.hpp"
#include <inttypes.h>
#include <string>

class BGDInternal;

namespace bgd {
    using address_t = uintptr_t;

    class BGDPluginBase;
    class BGDPlugin;
    class BGDHook;

    struct BGDHookInfo {
        const BGDHook* hook;
        std::string    module;

        BGDHookInfo() = delete;
        BGDHookInfo(BGDHookInfo const&) = delete;
        BGDHookInfo(BGDHook* hook);

        std::string formatted() const;
        std::string formattedDetails() const;
    };

    class BGD_DLL BGDHook {
        private:
            void* m_nAddress;
            void* m_pDetour;
            void* m_pHandle = nullptr;
            bool m_bEnabled;

            friend class BGDPlugin;
            friend class BGDPluginBase;
            friend class BGDInternal;
            friend struct BGDHookInfo;

        public:
            inline address_t getAddress() const {
                return reinterpret_cast<address_t>(m_nAddress);
            }
            inline bool isEnabled() const {
                return m_bEnabled;
            }

            BGDHook(address_t addr) :
                m_nAddress(reinterpret_cast<void*>(addr)), m_bEnabled(false) {}
            BGDHook(void* addr) :
                m_nAddress(addr), m_bEnabled(false) {}
    };
}

