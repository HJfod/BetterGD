#include "bgd_hook.hpp"

bool MenuLayer_init(MenuLayer* self) {
    if (!matdash::orig<&MenuLayer_init>(self))
        return false;
    
    FLAlertLayer::create(
        nullptr,
        "wow",
        "ok", nullptr,
        "yeah"
    )->show();
    
    return true;
}
static CreateHook<&MenuLayer_init>$mli(base + 0x1907b0);

void MenuLayer_onMoreGames(MenuLayer* self, CCObject* pSender) {
    auto np = BGDLoader::get()->updatePlugins();

    std::string text = std::to_string(np) + " plugins loaded\n";
    
    for (auto const& err : BGDLoader::get()->getErrors()) {
        // did you know?
        // std::string_view + std::string does not exist
        // :-)
        text += err.info;
        text += " -> ";
        text += err.description;
        text += "\n";
    }

    FLAlertLayer::create(
        nullptr,
        "wow",
        "ok", nullptr,
        text
    )->show();
}
static CreateHook<&MenuLayer_onMoreGames>$mlomg(base + 0x1919c0);

