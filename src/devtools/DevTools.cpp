#define BGD_INCLUDE_IMGUI
#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <limits>
#include "ArgParser.hpp"

#include "fonts/OpenSans.hpp"
#include "fonts/BGDIcons.hpp"
#include "fonts/FeatherIcons.hpp"
#include "fonts/RobotoMono.hpp"
#include "fonts/SourceCodeProLight.hpp"
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
    static const ImWchar icon_ranges[] = { FEATHER_MIN_FA, FEATHER_MAX_FA, 0 };
    static const ImWchar box_ranges[]  = { BOX_DRAWING_MIN_FA, BOX_DRAWING_MAX_FA, 0 };
    static const ImWchar* def_ranges   = ImGui::GetIO().Fonts->GetGlyphRangesDefault();
    
    static const auto add_font = +[](
        ImFont** member, void* font, float size, const ImWchar* range
    ) -> void {
        auto& io = ImGui::GetIO();
        ImFontConfig config;
        config.MergeMode = true;
        *member = io.Fonts->AddFontFromMemoryTTF(
            font, sizeof font, size, nullptr, range
        );
        io.Fonts->AddFontFromMemoryTTF(
            Font_FeatherIcons, sizeof Font_FeatherIcons, size - 4.f, &config, icon_ranges
        );
        io.Fonts->Build();
    };

    add_font(&this->m_pDefaultFont, Font_OpenSans,          18.f, def_ranges);
    add_font(&this->m_pSmallFont,   Font_OpenSans,          10.f, def_ranges);
    add_font(&this->m_pMonoFont,    Font_RobotoMono,        18.f, def_ranges);
    add_font(&this->m_pBoxFont,     Font_SourceCodeProLight,23.f, box_ranges);
}

DevTools::DevTools() {
    this->m_pColorYes       = new ImVec4;
    this->m_pColorNo        = new ImVec4;
    this->m_pColorWarning   = new ImVec4;
    this->m_pAddresses      = new AddressManager;
}

DevTools::~DevTools() {
    delete this->m_pColorYes;
    delete this->m_pColorNo;
    delete this->m_pColorWarning;
    delete this->m_pAddresses;
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

void DevTools::selectNode(CCNode* node) {
    CC_SAFE_RELEASE(this->m_pSelectedNode);
    this->m_pSelectedNode = node;
    CC_SAFE_RETAIN(this->m_pSelectedNode);
}

void DevTools::reloadStyle() {
    this->m_bLoadedStyle = false;
    this->loadStyle();
}

void DevTools::fixSceneScale(CCScene* scene) {
    auto t = as<AccessSpecifiersAreForNerds*>(
        dynamic_cast<CCTransitionScene*>(scene)
    );
    if (t) {
        scene = t->getIn();
    }
}

void DevTools::willSwitchToScene(CCScene* scene) {
    this->selectNode(nullptr);
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
    
    auto args = parser.value();

    SWITCH_ARGS {
        HANDLER("goto") {
            if (!args.hasArg(1)) {
                BGDInternalPlugin::get()->throwError(
                    "Invalid Command: \"goto\" requires a second parameter of type SceneID",
                    kBGDSeverityError
                );
            } else {
                auto scene = createSceneByLayerName(
                    args.at(1), as<void*>(std::stoi(args.at(2, "0")))
                );
                if (scene) {
                    BGDInternalPlugin::get()->log() << "Moving to scene " << args.at(1) << bgd::endl;
                    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
                    this->m_bCommandSuccess = true;
                } else {
                    BGDInternalPlugin::get()->log() << kBGDSeverityError << kBGDLogTypeError <<
                        "Invalid Command: \"" << args.at(1) << "\" is not a valid SceneID" << bgd::endl;
                }
            }
        }
    
        SWITCH_SUB("test") {
            HANDLER("warn") {
                BGDInternalPlugin::get()->log()
                    << kBGDSeverityWarning
                    << "Example warning"
                    << bgd::endl;
            }

            UNKNOWN_HANDLER();
        }
    
        UNKNOWN_HANDLER();
    }
}
