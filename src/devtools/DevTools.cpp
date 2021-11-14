#define BGD_INCLUDE_IMGUI
#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <limits>
#include "ArgParser.hpp"

#include "OpenSans.hpp"
#include "BGDIcons.hpp"
#include "FeatherIcons.hpp"
#undef max

constexpr static auto resource_dir = const_join_path<bgd_directory, bgd_resource_directory>;
constexpr static auto font_default = std::string_view("OpenSans-Regular.ttf");

bool DevTools::isVisible() const {
    return this->m_bVisible;
}

bool DevTools::shouldPopGame() const {
    return this->m_bVisible && this->m_bGDInWindow;
}

void DevTools::draw() {
    if (this->m_bVisible) {
        auto& style = ImGui::GetStyle();
        style.ColorButtonPosition = ImGuiDir_Left;

        this->loadStyle();

        ImGuiWindowFlags flags = 
            ImGuiWindowFlags_NoScrollbar;
        
        this->m_nDockSpaceID = ImGui::DockSpaceOverViewport(
            nullptr, ImGuiDockNodeFlags_PassthruCentralNode
        );
        auto& fonts = ImGui::GetIO().Fonts->Fonts;
        ImGui::PushFont(this->m_pDefaultFont);
        this->generateTabs();
        ImGui::PopFont();
        this->fixSceneScale(CCDirector::sharedDirector()->getRunningScene());
    }
}

void DevTools::initFonts() {
    auto& io = ImGui::GetIO();
    static const ImWchar icon_ranges[] = { FEATHER_MIN_FA, FEATHER_MAX_FA, 0 };

    ImFontConfig defConfig;
    defConfig.MergeMode = true;
    this->m_pDefaultFont = io.Fonts->AddFontFromMemoryTTF(
        Font_OpenSans, sizeof Font_OpenSans, 18.f
    );
    io.Fonts->AddFontFromMemoryTTF(
        Font_FeatherIcons, sizeof Font_FeatherIcons, 14.f, &defConfig, icon_ranges
    );
    io.Fonts->Build();

    ImFontConfig smallConfig;
    smallConfig.MergeMode = true;
    this->m_pSmallFont = io.Fonts->AddFontFromMemoryTTF(
        Font_OpenSans, sizeof Font_OpenSans, 10.f
    );
    io.Fonts->AddFontFromMemoryTTF(
        Font_FeatherIcons, sizeof Font_FeatherIcons, 6.f, &smallConfig, icon_ranges
    );
    io.Fonts->Build();
}

DevTools::DevTools() {
    this->m_pColorYes       = new ImVec4;
    this->m_pColorNo        = new ImVec4;
    this->m_pColorWarning   = new ImVec4;
}

DevTools::~DevTools() {
    delete this->m_pColorYes;
    delete this->m_pColorNo;
    delete this->m_pColorWarning;
}

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

void DevTools::updateSceneScale(CCScene* scene) {
    if (!ImGui::GetCurrentContext())
        return;

    auto win = ImGui::GetMainViewport()->Size;

    enum docking {
        left        = 0b1,
        top         = 0b10,
        right       = 0b100,
        bottom      = 0b1000,
    };

    for (auto const& info : this->m_vDockInfo) {
        int dock = 0;
        if (info.origin.x <= 0.f) {
            dock |= left;
        }
        if (info.origin.y <= 0.f) {
            dock |= top;
        }
        if (info.origin.x + info.size.width >= win.x) {
            dock |= right;
        }
        if (info.origin.y + info.size.height >= win.y) {
            dock |= bottom;
        }
    }
}

void DevTools::fixSceneScale(CCScene* scene) {
    auto t = as<AccessSpecifiersAreForNerds*>(
        dynamic_cast<CCTransitionScene*>(scene)
    );
    if (t) {
        scene = t->getIn();
        this->updateSceneScale(t->getOut());
    }
    this->updateSceneScale(scene);
}

void DevTools::willSwitchToScene(CCScene* scene) {
    this->fixSceneScale(scene);
}

void DevTools::show() {
    if (!this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = true;
    }
}

void DevTools::hide() {
    if (this->m_bVisible) {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        this->m_bVisible = false;
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

    auto parser = ArgParser().parse(cmd);

    if (!parser) {
        BGDInternalPlugin::get()->log()
            << kBGDLogTypeError << kBGDSeverityError
            << "Error Parsing Command: " << parser.error()
            << bgd::endl;
        return;
    }
    
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
                    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
                    this->m_bCommandSuccess = true;
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
