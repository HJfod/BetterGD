#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <imgui-hook.hpp>
#include <limits>
#include "OpenSans.hpp"
#undef max

constexpr static auto resource_dir = const_join_path<bgd_directory, bgd_resource_directory>;
constexpr static auto font_default = std::string_view("OpenSans-Regular.ttf");

void DevTools::resizeWindow() {
    auto win = ImGui::GetMainViewport()->Size;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    switch (this->m_eMount) {
        case kDevToolsMountWest: {
            auto ratio     = this->m_fWidth    / winSize.width;
            auto ratio_min = this->m_fMinWidth / winSize.width;
            auto ratio_max = this->m_fMaxWidth / winSize.width;
            ImGui::SetNextWindowPos({ 0, 0 });
            ImGui::SetNextWindowSize({
                win.x * ratio * this->getSceneScale(), win.y
            });
            ImGui::SetNextWindowSizeConstraints({
                win.x * ratio_min * this->getSceneScale(), win.y
            }, {
                win.x * ratio_max * this->getSceneScale(), win.y
            });
        } break;
            
        case kDevToolsMountEast: {
            auto ratio     = this->m_fWidth    / winSize.width;
            auto ratio_min = this->m_fMinWidth / winSize.width;
            auto ratio_max = this->m_fMaxWidth / winSize.width;
            ImGui::SetNextWindowPos({
                win.x - win.x * ratio * this->getSceneScale(), 0
            });
            ImGui::SetNextWindowSize({
                win.x * ratio * this->getSceneScale(), win.y
            });
            ImGui::SetNextWindowSizeConstraints({
                win.x * ratio_min * this->getSceneScale(), win.y
            }, {
                win.x * ratio_max * this->getSceneScale(), win.y
            });
        } break;
            
        case kDevToolsMountSouth: {
            auto ratio     = this->m_fHeight    / winSize.height;
            auto ratio_min = this->m_fMinHeight / winSize.height;
            auto ratio_max = this->m_fMaxHeight / winSize.height;
            ImGui::SetNextWindowPos({
                0, win.y - win.y * ratio * this->getSceneScale()
            });
            ImGui::SetNextWindowSize({
                win.x, win.y * ratio * this->getSceneScale()
            });
            ImGui::SetNextWindowSizeConstraints({
                win.x, win.y * ratio_min * this->getSceneScale()
            }, {
                win.x, win.y * ratio_max * this->getSceneScale()
            });
        } break;
            
        case kDevToolsMountNorth: {
            auto ratio     = this->m_fHeight    / winSize.height;
            auto ratio_min = this->m_fMinHeight / winSize.height;
            auto ratio_max = this->m_fMaxHeight / winSize.height;
            ImGui::SetNextWindowPos({
                0, 0
            });
            ImGui::SetNextWindowSize({
                win.x, win.y * ratio * this->getSceneScale()
            });
            ImGui::SetNextWindowSizeConstraints({
                win.x, win.y * ratio_min * this->getSceneScale()
            }, {
                win.x, win.y * ratio_max * this->getSceneScale()
            });
        } break;
    }
}

void DevTools::hideOverflow() {
    // no not the hentai
    if (!this->m_bHideOverflow || this->m_eMode == kDevToolsModePopup) {
        return;
    }

    auto win = ImGui::GetMainViewport()->Size;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto list = ImGui::GetForegroundDrawList();

    switch (this->m_eMount) {
        case kDevToolsMountEast: case kDevToolsMountWest: {
            float height = (win.y - (win.x - ImGui::GetWindowWidth()) * win.y / win.x) / 2;
            const float offset_l = this->m_eMount == kDevToolsMountWest ?
                ImGui::GetWindowWidth() : 0;
            const float offset_r = this->m_eMount == kDevToolsMountEast ?
                ImGui::GetWindowWidth() : 0;

            list->AddRectFilled({
                offset_l, 0
            }, {
                win.x - offset_r,
                height
            }, 0xff000000);

            list->AddRectFilled({
                offset_l,
                win.y - height
            }, {
                win.x - offset_r,
                win.y
            }, 0xff000000);
        } break;

        case kDevToolsMountSouth: case kDevToolsMountNorth: {
            float width = (win.x - (win.y - ImGui::GetWindowHeight()) * win.x / win.y) / 2;
            const float offset_t = this->m_eMount == kDevToolsMountNorth ?
                ImGui::GetWindowHeight() : 0;
            const float offset_b = this->m_eMount == kDevToolsMountSouth ?
                ImGui::GetWindowHeight() : 0;

            list->AddRectFilled({
                0, offset_t
            }, {
                width,
                win.y - offset_b,
            }, 0xff000000);

            list->AddRectFilled({
                win.x - width,
                offset_t
            }, {
                win.x,
                win.y - offset_b
            }, 0xff000000);
        } break;
    }
}

void DevTools::draw() {
    if (this->m_bVisible) {
        auto& style = ImGui::GetStyle();
        style.ColorButtonPosition = ImGuiDir_Left;

        this->loadStyle();

        ImGuiWindowFlags flags = 
            ImGuiWindowFlags_HorizontalScrollbar;
        
        if (this->m_eMode == kDevToolsModeIntegrated) {
            flags |=
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse;
        }

        if (this->m_eMode == kDevToolsModeIntegrated) {
            this->resizeWindow();
        }
        if (ImGui::Begin("BetterGD Dev Tools", nullptr, flags)) {
            auto win = ImGui::GetMainViewport()->Size;
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            ImGui::PushCheckboxSize(.7f);

            this->hideOverflow();

            auto& fonts = ImGui::GetIO().Fonts->Fonts;
            ImGui::PushFont(this->m_pDefaultFont);
            this->generateContent();
            ImGui::PopFont();

            if (this->m_eMode == kDevToolsModeIntegrated) {
                if (
                    this->m_eMount == kDevToolsMountEast ||
                    this->m_eMount == kDevToolsMountWest
                ) {
                    this->m_fWidth = 
                        (ImGui::GetWindowWidth()  * winSize.width) /
                        (win.x * this->getSceneScale());
                }
                if (
                    this->m_eMount == kDevToolsMountSouth ||
                    this->m_eMount == kDevToolsMountNorth
                ) {
                    this->m_fHeight =
                        (ImGui::GetWindowHeight() * winSize.height) /
                        (win.y * this->getSceneScale());
                }
            }
            this->showAnimation(CCDirector::sharedDirector()->getRunningScene(), false);
        }
        ImGui::End();
    }
}

DevTools::DevTools() {
    ImGuiHook::setRenderFunction([this]() -> void { this->draw(); });
    ImGuiHook::setToggleCallback([this]() -> void {
        this->m_bVisible ^= 1;
    });
    ImGuiHook::setupHooks([](void* target, void* hook, void** trampoline) -> void {
        BGDInternalPlugin::get()->addHookInternal(target, hook, trampoline);
    });
    ImGuiHook::setInitFunction([this]() -> void {
        this->m_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Font_OpenSans, sizeof Font_OpenSans, 18.f);
        this->m_pSmallFont   = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Font_OpenSans, sizeof Font_OpenSans, 10.f);
    });
}

DevTools::~DevTools() {}

DevTools* DevTools::get() {
    static auto g_dev = new DevTools;
    return g_dev;
}

class AccessSpecifiersAreForNerds : public CCTransitionScene {
    public:
        CCScene* getIn()  { return this->m_pInScene; }
        CCScene* getOut() { return this->m_pOutScene; }
};

void DevTools::reloadStyle() {
    this->m_bLoadedStyle = false;
    this->loadStyle();
}

void DevTools::updateVisibility(DevToolsMode mode, DevToolsMount mount) {
    if (this->m_eMode == kDevToolsModePopup) {
        this->m_obPopoutSize.width  = ImGui::GetWindowSize().x;
        this->m_obPopoutSize.height = ImGui::GetWindowSize().y;
    }
    if (mode == kDevToolsModePopup) {
        ImGui::SetWindowSize({
            this->m_obPopoutSize.width,
            this->m_obPopoutSize.height
        });
    } else {
        ImGui::SetWindowSize({
            this->m_fWidth,
            this->m_fHeight
        });
    }
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    this->m_eMode = mode;
    this->m_eMount = mount;
    this->showAnimation(scene, false);
    this->reloadStyle();
}

float DevTools::getSceneScale() {
    if (this->m_eMode == kDevToolsModePopup || !this->m_bVisible) {
        return 1.f;
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if (
        this->m_eMount == kDevToolsMountSouth ||
        this->m_eMount == kDevToolsMountNorth
    ) {
        return winSize.height / (winSize.height + this->m_fHeight);
    }
    return winSize.width / (winSize.width + this->m_fWidth);
}

void DevTools::showAnimation(CCScene* scene, bool transition) {
    auto scale = getSceneScale();
    switch (this->m_eMount) {
        case kDevToolsMountWest:
            scene->setAnchorPoint({ 1.f, .5f });
            break;
        case kDevToolsMountEast:
            scene->setAnchorPoint({ .0f, .5f });
            break;
        case kDevToolsMountNorth:
            scene->setAnchorPoint({ .5f, .0f });
            break;
        case kDevToolsMountSouth:
            scene->setAnchorPoint({ .5f, 1.f });
            break;
    }
    transition = false;
    if (transition) {
        scene->runAction(CCEaseInOut::create(
            CCScaleTo::create(.5f, scale), 2.f
        ));
    } else {
        scene->setScale(scale);
    }
}

void DevTools::fixSceneScale(CCScene* scene) {
    auto t = as<AccessSpecifiersAreForNerds*>(
        dynamic_cast<CCTransitionScene*>(scene)
    );
    if (t) {
        scene = t->getIn();
        this->showAnimation(t->getOut(), false);
    }
    this->showAnimation(scene, false);
}

void DevTools::willSwitchToScene(CCScene* scene) {
    this->fixSceneScale(scene);
}

void DevTools::show() {
    if (!this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = true;
        this->showAnimation(scene, true);
    }
}

void DevTools::hide() {
    if (this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = false;
        this->showAnimation(scene, true);
    }
}

void DevTools::toggle() {
    if (this->m_bVisible) {
        this->hide();
    } else {
        this->show();
    }
}

void split_in_args(std::vector<std::string>& qargs, std::string command){
    int len = command.length();
    bool qot = false, sqot = false;
    int arglen;
    for(int i = 0; i < len; i++) {
            int start = i;
            if(command[i] == '\"') {
                    qot = true;
            }
            else if(command[i] == '\'') sqot = true;

            if(qot) {
                    i++;
                    start++;
                    while(i<len && command[i] != '\"')
                            i++;
                    if(i<len)
                            qot = false;
                    arglen = i-start;
                    i++;
            }
            else if(sqot) {
                    i++;
                    while(i<len && command[i] != '\'')
                            i++;
                    if(i<len)
                            sqot = false;
                    arglen = i-start;
                    i++;
            }
            else{
                    while(i<len && command[i]!=' ')
                            i++;
                    arglen = i-start;
            }
            qargs.push_back(command.substr(start, arglen));
    }
}

CCScene* createSceneByLayerName(std::string const& name, void* param = nullptr) {
    auto scene = CCScene::create();
    switch (hash(name.c_str())) {
        case "MenuLayer"_h:
            scene->addChild(MenuLayer::create());
            break;

        case "GJGarageLayer"_h:
            scene->addChild(GJGarageLayer::create());
            break;

        case "LevelSelectLayer"_h:
            scene->addChild(LevelSelectLayer::create(as<int>(param)));
            break;

        case "LevelBrowserLayer"_h:
            scene->addChild(LevelBrowserLayer::create(GJSearchObject::create(cast::enum_cast<SearchType>(param))));
            break;

        default:
            CC_SAFE_RELEASE(scene);
            return nullptr;
    }
    return scene;
}

void DevTools::executeConsoleCommand(std::string const& cmd) {
    if (!cmd.size()) return;
    
    std::vector<std::string> args;
    split_in_args(args, cmd);

    switch (hash(args[0].c_str())) {
        case "goto"_h: {
            if (args.size() < 2) {
                BGDInternalPlugin::get()->throwError(
                    "Invalid Command: \"goto\" requires a second parameter of type SceneID",
                    kBGDSeverityError
                );
            } else {
                auto scene = createSceneByLayerName(
                    args[1], as<void*>(args.size() < 3 ? 0 : std::stoi(args[2]))
                );
                if (scene) {
                    BGDInternalPlugin::get()->log() << "Moving to scene " << args[1] << bgd::endl;
                    CCDirector::sharedDirector()->replaceScene(scene);
                } else {
                    BGDInternalPlugin::get()->log() << kBGDSeverityError << kBGDLogTypeError <<
                        "Invalid Command: \"" << args[1] << "\" is not a valid SceneID" << bgd::endl;
                }
            }
        } break;
    
        default:
            BGDInternalPlugin::get()->log() << kBGDSeverityError << kBGDLogTypeError <<
                "Unknown Command: \"" << args[0] << "\"" << bgd::endl;
    }
}
