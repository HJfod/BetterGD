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

    class BGD_DLL BGDLoader {
        protected:
            std::unordered_map<std::string, BGDPlugin*>* m_pLoadedPlugins;
            bool m_bIsSetup = false;

            BGDLoader();
            virtual ~BGDLoader();

            void createDirectories();
            void updatePlugins();
            void loadPluginFromFile(std::string const& file);
            
        public:
            static BGDLoader* get();
            bool setup();
    
            bool isPluginLoaded(std::string const& id);
            BGDPlugin* getLoadedPlugin(std::string const& id);
    };
}
