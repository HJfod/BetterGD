#pragma once

#include "BGDMacros.hpp"
#include "BGDPlugin.hpp"
#include <vector>

namespace bgd {
    static constexpr const char* bgd_directory          = "BetterGD";
    static constexpr const char* bgd_plugin_directory   = "plugins";
    static constexpr const char* bgd_plugin_extension   = "bgd";

    BGD_DLL class BGDLoader {
        protected:
            std::vector<BGDPlugin*> m_vLoadedPlugins;

            BGDLoader();
            virtual ~BGDLoader();

            void createDirectories();
            void updatePlugins();
            
        public:
            static BGDLoader* get();
    
            bool isPluginLoaded(std::string const& id);
            BGDPlugin* getLoadedPlugin(std::string const& id);
    };
}
