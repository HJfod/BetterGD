#pragma once

#include "BGDMacros.hpp"
#include "BGDError.hpp"
#include <string>

namespace bgd {
    class BGDLoader;

    struct BGDPlatformInfo;

    class BGD_DLL BGDPlugin {
        protected:
            const char* m_sName;
            const char* m_sDeveloper;
            const char* m_sDescription = nullptr;
            const char* m_sCredits = nullptr;
            const char* m_sID;
            // internal members; do NOT modify
            const char* m_sPath;
            BGDPlatformInfo* m_pInfo;
            
            void platformCleanup();

            virtual void setup() = 0;
            virtual void enable();
            virtual void disable();
            virtual void saveData();
            virtual void loadData();

            friend class BGDLoader;

        public:
            void throwError(BGDError const&);

            const char* getID()         const;
            const char* getName()       const;
            const char* getDeveloper()  const;
            const char* getCredits()    const;
            const char* getPath()       const;

            std::ostream& log();

            BGDPlugin();
            virtual ~BGDPlugin();
    };

    typedef bgd::BGDPlugin* (__stdcall* bgd_load_type)();
}
