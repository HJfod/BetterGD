#pragma once

#include <lilac.hpp>
#include "BGDUtils.hpp"

#include "nodes/BrownAlertDelegate.hpp"
#include "nodes/BGCheckbox.hpp"
#include "nodes/HorizontalSelectMenu.hpp"
#include "nodes/InputNode.hpp"
#include "nodes/InputPrompt.hpp"
#include "nodes/MonoSpaceLabel.hpp"
#include "nodes/PickMenu.hpp"
#include "nodes/Scrollbar.hpp"
#include "nodes/BGLabel.hpp"

#include "managers/SuperMouseManager.hpp"
#include "managers/SuperKeyboardManager.hpp"

namespace bgd {
    static constexpr const std::string_view bgd_directory          = "BetterGD";
    static constexpr const std::string_view bgd_plugin_directory   = "plugins";
    static constexpr const std::string_view bgd_resource_directory = "resources";
    static constexpr const std::string_view bgd_plugin_extension   = ".bgd";
    
    class BGD_DLL BetterGD : public Interface {
    public:
        static BetterGD* get();
        static Mod* mod();
    };
}
