#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <imgui-hook.hpp>
#include <limits>
#undef max

constexpr static auto resource_dir = const_join_path<bgd_directory, bgd_resource_directory>;
constexpr static auto font_default = std::string_view("OpenSans-Regular.ttf");

void DevTools::resizeWindow() {
    auto win = ImGui::GetMainViewport()->Size;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    switch (this->m_eMount) {
        case kDevToolsMountWest: {
            auto ratio = this->m_fWidth / winSize.width;
            ImGui::SetWindowPos({ 0, 0 });
            ImGui::SetWindowSize({
                win.x * ratio * this->getSceneScale(), win.y
            });
        } break;
            
        case kDevToolsMountEast: {
            auto ratio = this->m_fWidth / winSize.width;
            ImGui::SetWindowPos({
                win.x - win.x * ratio * this->getSceneScale(), 0
            });
            ImGui::SetWindowSize({
                win.x * ratio * this->getSceneScale(), win.y
            });
        } break;
            
        case kDevToolsMountSouth: {
            auto ratio = this->m_fHeight / winSize.height;
            ImGui::SetWindowPos({
                0, win.y - win.y * ratio * this->getSceneScale()
            });
            ImGui::SetWindowSize({
                win.x, win.y * ratio * this->getSceneScale()
            });
        } break;
            
        case kDevToolsMountNorth: {
            auto ratio = this->m_fHeight / winSize.height;
            ImGui::SetWindowPos({
                0, 0
            });
            ImGui::SetWindowSize({
                win.x, win.y * ratio * this->getSceneScale()
            });
        } break;
    }
}

void DevTools::draw() {
    auto tools = DevTools::get();
    if (tools->m_bVisible) {
        auto& style = ImGui::GetStyle();
        style.ColorButtonPosition = ImGuiDir_Left;

        tools->loadColorScheme();

        ImGuiWindowFlags flags = 
            ImGuiWindowFlags_HorizontalScrollbar;
        
        if (tools->m_eMode == kDevToolsModeIntegrated) {
            flags |=
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse;
        }

        auto director = CCDirector::sharedDirector();
        if (ImGui::Begin("BetterGD Dev Tools", nullptr, flags)) {
            if (tools->m_eMode == kDevToolsModeIntegrated) {
                tools->resizeWindow();
            }
            tools->generateContent();
        }
        ImGui::End();
    }
}

DevTools::DevTools() {
    ImGuiHook::setRenderFunction(DevTools::draw);
    ImGuiHook::setToggleCallback([this]() {
        this->m_bVisible ^= 1;
    });
    ImGuiHook::setupHooks([](void* target, void* hook, void** trampoline) {
        BGDInternalPlugin::get()->addHookInternal(target, hook, trampoline);
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

void DevTools::loadColorScheme() {
    auto style = &ImGui::GetStyle();
    auto colors = style->Colors;
    
    if (this->m_eMode == kDevToolsModeIntegrated) {
        style->WindowRounding    = 0.0f;
    } else {
        style->WindowRounding    = 2.0f;
    }
    style->ScrollbarRounding = 3.0f;
    style->GrabRounding      = 2.0f;
    style->AntiAliasedLines  = true;
    style->AntiAliasedFill   = true;
    style->WindowRounding    = 2;
    style->ChildRounding     = 2;
    style->ScrollbarSize     = 16;
    style->ScrollbarRounding = 3;
    style->GrabRounding      = 2;
    style->ItemSpacing.x     = 10;
    style->ItemSpacing.y     = 4;
    style->IndentSpacing     = 22;
    style->FramePadding.x    = 6;
    style->FramePadding.y    = 4;
    style->Alpha             = 1.0f;
    style->FrameRounding     = 3.0f;

    colors[ImGuiCol_Text]                  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.93f, 0.93f, 0.93f, 0.98f);
    if (this->m_eMode == kDevToolsModeIntegrated) {
        colors[ImGuiCol_Border] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_BorderShadow] = colors[ImGuiCol_WindowBg];
    } else {
        colors[ImGuiCol_Border]                = ImVec4(0.71f, 0.71f, 0.71f, 0.08f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
    }
    colors[ImGuiCol_FrameBg]               = ImVec4(0.71f, 0.71f, 0.71f, 0.55f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.94f, 0.94f, 0.94f, 0.55f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.71f, 0.78f, 0.69f, 0.98f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.82f, 0.78f, 0.78f, 0.51f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.61f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.90f, 0.90f, 0.90f, 0.30f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.92f, 0.92f, 0.92f, 0.78f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.184f, 0.407f, 0.193f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.71f, 0.78f, 0.69f, 0.40f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.725f, 0.805f, 0.702f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.793f, 0.900f, 0.836f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.71f, 0.78f, 0.69f, 0.31f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.71f, 0.78f, 0.69f, 0.80f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.71f, 0.78f, 0.69f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]        = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_NavHighlight]          = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);

    constexpr auto file = const_join_path_c_str<resource_dir, font_default>;
    auto path = std::filesystem::absolute(file);

    std::cout << path << "\n";
    if (std::filesystem::exists(path)) {
        ImGuiIO& io = ImGui::GetIO();
        auto font = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 13);
        std::cout << "loadin\n";
        if (font && font->IsLoaded()) {
            std::cout << "pushin";
            ImGui::PushFont(font);
        } else {
            std::cout << "fuck\n";
        }
    }
}

void DevTools::recurseUpdateList(CCNode* parent) {

}

void DevTools::generateContent() {
    if (ImGui::ArrowButton("dev.mount_west", ImGuiDir_Left)) {
        this->updateVisibility(kDevToolsModeIntegrated, kDevToolsMountWest);
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("dev.mount_east", ImGuiDir_Right)) {
        this->updateVisibility(kDevToolsModeIntegrated, kDevToolsMountEast);
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("dev.mount_south", ImGuiDir_Down)) {
        this->updateVisibility(kDevToolsModeIntegrated, kDevToolsMountSouth);
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("dev.mount_north", ImGuiDir_Up)) {
        this->updateVisibility(kDevToolsModeIntegrated, kDevToolsMountNorth);
    }
    ImGui::SameLine();
    if (ImGui::Button("Pop out")) {
        this->updateVisibility(kDevToolsModePopup);
    }
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
    }
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    this->m_eMode = mode;
    this->m_eMount = mount;
    this->showAnimation(scene, false);
}

float DevTools::getSceneScale() {
    if (this->m_eMode == kDevToolsModePopup) {
        return 1.f;
    }
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    return this->m_bVisible ?
        winSize.width / (winSize.width + this->m_fWidth) :
        1.f;
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
