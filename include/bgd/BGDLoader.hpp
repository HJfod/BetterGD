#pragma once

#include "BGDMacros.hpp"
#include "BGDPlugin.hpp"
#include <vector>
#include <unordered_map>

namespace bgd {
    static constexpr const char* bgd_directory          = "BetterGD";
    static constexpr const char* bgd_plugin_directory   = "plugins";
    static constexpr const char* bgd_plugin_extension   = ".bgd";

    class BGDPlugin;

    #pragma warning(disable: 4251) // i will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)

    class BGD_DLL BGDLoader {
        protected:
            std::vector<BGDPlugin*> m_vLoadedPlugins;
            std::vector<BGDError> m_vErrors;
            bool m_bIsSetup = false;

            BGDLoader();
            virtual ~BGDLoader();

            void createDirectories();
            bool loadPluginFromFile(std::string const& file);
            
        public:
            static BGDLoader* get();
            bool setup();
            size_t updatePlugins();

            void saveData();
            void loadData();

            void throwError(BGDError const&);
            std::vector<BGDError> getErrors(
                std::initializer_list<BGDErrorType> typeFilter     = {},
                std::initializer_list<BGDSeverity>  severityFilter = {}
            );
    
            bool isPluginLoaded(std::string const& id);
            BGDPlugin* getLoadedPlugin(std::string const& id);
            std::vector<BGDPlugin*> getAllPlugins();
    };
}
