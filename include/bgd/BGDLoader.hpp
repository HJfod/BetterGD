#pragma once

#include "BGDMacros.hpp"
#include "BGDPlugin.hpp"
#include <vector>
#include <unordered_map>

namespace bgd {
    static constexpr const std::string_view bgd_directory          = "BetterGD";
    static constexpr const std::string_view bgd_plugin_directory   = "plugins";
    static constexpr const std::string_view bgd_resource_directory = "resources";
    static constexpr const std::string_view bgd_plugin_extension   = ".bgd";

    class BGDPlugin;

    class BGD_DLL BGDLoader {
        protected:
            std::vector<BGDPlugin*> m_vLoadedPlugins;
            std::vector<BGDLog*> m_vLogs;
            BGDLogStream* m_pLogStream;
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

            inline BGDLogStream& logStream() {
                return *this->m_pLogStream;
            }

            void log(BGDLog* log);
            std::vector<BGDLog*>      getLogs() const;
            std::vector<BGDLogError*> getErrors(
                std::initializer_list<BGDErrorType> typeFilter     = {},
                std::initializer_list<BGDSeverity>  severityFilter = {}
            );
    
            bool isPluginLoaded(std::string const& id);
            BGDPlugin* getLoadedPlugin(std::string const& id);
            std::vector<BGDPlugin*> getAllPlugins();
    };
}
