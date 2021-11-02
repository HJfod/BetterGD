#pragma once

#include <inttypes.h>

namespace bgd {
    using address_t = uintptr_t;

    class BGD_DLL BGDHook {
        private:
            address_t m_nAddress;
            bool m_bEnabled;

        public:
            inline address_t getAddress() const {
                return m_nAddress;
            }
            inline bool isEnabled() const {
                return m_bEnabled;
            }

            BGDHook(address_t addr, bool enabled) :
                m_nAddress(addr), m_bEnabled(enabled) {}
    };
}

