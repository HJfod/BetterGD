#include "BGDInternal.hpp"
#include <BGDLoader.hpp>
#include <tools/keybinds/KeybindManager.hpp>
#include "../devtools/DevTools.hpp"

using namespace std::string_view_literals;

USE_BGD_NAMESPACE();

void BGDInternal::addResourceSearchPaths() {
    auto utils = CCFileUtils::sharedFileUtils();
    auto path = utils->fullPathForFilename(const_join_path_c_str<bgd_directory, bgd_resource_directory>, false);
    if (!vector_contains<std::string>(utils->getSearchPaths(), path)) {
        utils->addSearchPath(path.c_str());
    }
}

void BGDInternal::loadKeybinds() {
    KeybindManager::get()->addGlobalKeybindAction(TriggerableAction {
        "Toggle Dev Tools",
        "bgd.toggle_dev_tools",
        "bgd.dev",
        [](CCNode* scene, bool down) -> bool {
            if (down) {
                DevTools::get()->toggle();
            }
            return false;
        }
    }, {{ KEY_I, Keybind::kmControl | Keybind::kmShift }});

    KeybindManager::get()->addGlobalKeybindAction(TriggerableAction {
        "Reload Plugins",
        "bgd.reload_plugins",
        "bgd.dev",
        [](CCNode*, bool down) -> bool {
            if (down) {
                auto u = BGDLoader::get()->updatePlugins();
                FLAlertLayer::create(
                    nullptr,
                    "Plugins Updated",
                    "OK", nullptr,
                    std::to_string(u) + " plugins loaded"
                )->show();
            }
            return false;
        }
    }, {{ KEY_R, Keybind::kmControl | Keybind::kmShift }});

    // BGDInternalPlugin::get()->throwError(
    //     "test", "test error", kBGDSeverityAlert, kBGDErrorTypeGeneral
    // );
}

bool BGDInternal::isFileInSearchPaths(const char* file) {
    auto utils = CCFileUtils::sharedFileUtils();
    return utils->isFileExist(utils->fullPathForFilename(file, false));
}

void BGDInternal::setup() {
    this->loadKeybinds();
    this->addResourceSearchPaths();
}

BGDInternal* BGDInternal::get() {
    static auto g_int = new BGDInternal;
    return g_int;
}

void BGDInternalPlugin::setup() {
    this->m_sID = "hjfod.bettergd";
    this->m_sName = "BetterGD";
    this->m_sDeveloper = "HJfod";
}

BGDInternalPlugin* BGDInternalPlugin::get() {
    static auto g_plugin = new BGDInternalPlugin;
    return g_plugin;
}
