#pragma once

#include "BGDMacros.hpp"
#include <string>
#include <utils/gd/include_gd.hpp>
#undef snprintf
#include <external/json.hpp>

namespace bgd {
    class BGDPlugin;

    enum BGDSaveType {
        kBGDSaveTypeDSDictionary,
        kBGDSaveTypeJSON,
        kBGDSaveTypeCustom,
    };

    class BGD_DLL BGDSaveManager {
        protected:
            BGDPlugin* m_pOwner = nullptr;
            std::string m_sSaveDirectory;
            std::string m_sSaveFileName;
            BGDSaveType m_eSaveType;

            virtual void firstSetup();
            virtual void saveDict(DS_Dictionary*);
            virtual void loadDict(DS_Dictionary*);
            virtual void saveJSON(nlohmann::json &);
            virtual void loadJSON(nlohmann::json &);
            virtual void saveCustom(std::string const& path);
            virtual void loadCustom(std::string const& path);

            void setup(BGDPlugin*);

            BGDSaveManager();
            virtual ~BGDSaveManager();

        public:
            void saveData(std::string const& dir);
            void loadData(std::string const& dir);
    };
}
