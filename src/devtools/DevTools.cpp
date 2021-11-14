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

void DevTools::hideOverflow() {
    // no not the hentai
    if (!this->m_bHideOverflow || this->m_eMode == kDevToolsModePopup) {
        return;
    }

    // todo: do this using Cocos2d
}

void DevTools::draw() {
    if (this->m_bVisible) {

    }
}

DevTools::DevTools() {}

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
    if (this->m_eMode == kDevToolsModePopup) {}
    if (mode == kDevToolsModePopup) {
        // todo
    } else {
        // todo
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
