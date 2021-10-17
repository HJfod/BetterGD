#pragma once

#include "BGDMacros.hpp"
#include "BGDPlugin.hpp"
#include <vector>

namespace bgd {
    BGD_DLL class BGDLoader {
        protected:
            std::vector<BGDPlugin*> m_vLoadedPlugins;

            BGDLoader();
            virtual ~BGDLoader();
            
        public:
            static BGDLoader* get();
    
            bool isPluginLoaded(std::string const& id);
            BGDPlugin* getLoadedPlugin(std::string const& id);
    };
}
