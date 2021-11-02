#pragma once

#include "BGDMacros.hpp"
#include "BGDError.hpp"
#include "BGDHook.hpp"
#include <string>
#include <vector>

class BGDInternal;

namespace bgd {
    class BGDLoader;
    class BGDSaveManager;

    struct BGDPlatformInfo;

    #pragma warning(disable: 4251) // i will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)
                                   
    class BGD_DLL BGDPluginBase {
        protected:
            const char* m_sPath;
            BGDPlatformInfo* m_pInfo;
            std::vector<BGDSaveManager*> m_vSaveManagers;
            std::vector<BGDHook*> m_vHooks;
    };

    class BGD_DLL BGDPlugin : BGDPluginBase {
        protected:
            const char* m_sID;
            const char* m_sName;
            const char* m_sDeveloper;
            const char* m_sDescription = nullptr;
            const char* m_sCredits = nullptr;

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
            friend class BGDInternal;

            void addHook(BGDHook* hook);

        public:
            void throwError(BGDError const& error);

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
