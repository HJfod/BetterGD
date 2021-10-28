#pragma once

#include "BGDMacros.hpp"
#include "BGDError.hpp"
#include <string>
#include <vector>

namespace bgd {
    class BGDLoader;
    class BGDSaveManager;

    struct BGDPlatformInfo;

    #pragma warning(disable: 4251) // i will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)
                                   
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
            std::vector<BGDSaveManager*> m_vSaveManagers;

            void platformCleanup();

            virtual void setup() = 0;
            virtual void enable();
            virtual void disable();
            virtual void saveData();
            virtual void loadData();

            void registerSaveManager(BGDSaveManager*);
            void unregisterSaveManager(BGDSaveManager*);

            friend class BGDLoader;
            friend class BGDSaveManager;

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
