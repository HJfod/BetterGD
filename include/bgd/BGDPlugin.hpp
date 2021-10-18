#pragma once

#include "BGDMacros.hpp"
#include <string>

namespace bgd {
    class BGDLoader;

    class BGD_DLL BGDPlugin {
        protected:
            std::string m_sName;
            std::string m_sID;
            
            virtual void setup() = 0;
            virtual void enable();
            virtual void disable();

            friend class BGDLoader;

        public:
            BGDPlugin();
            virtual ~BGDPlugin();
    };

    typedef bgd::BGDPlugin* (__stdcall* bgd_load_type)();
}
