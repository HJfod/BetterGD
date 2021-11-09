#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <imgui-hook.hpp>
#include <limits>
#include "OpenSans.hpp"
#undef max

constexpr static auto resource_dir = const_join_path<bgd_directory, bgd_resource_directory>;
constexpr static auto font_default = std::string_view("OpenSans-Regular.ttf");

ImVec2 toVec2(const CCPoint& a) {
    const auto size = ImGui::GetMainViewport()->Size;
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    return {
        a.x / winSize.width * size.x,
        (1.f - a.y / winSize.height) * size.y
    };
}

ImVec2 toVec2(const CCSize& a) {
    const auto size = ImGui::GetMainViewport()->Size;
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    return {
        a.width / winSize.width * size.x,
        -a.height / winSize.height * size.y
    };
}

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

        tools->loadStyle();

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
            auto& fonts = ImGui::GetIO().Fonts->Fonts;
            ImGui::PushFont(fonts[fonts.size() - 1]);
            if (tools->m_eMode == kDevToolsModeIntegrated) {
                tools->resizeWindow();
            }
            tools->generateContent();
            ImGui::PopFont();
        }
        ImGui::End();
    }
}

DevTools::DevTools() {
    ImGuiHook::setRenderFunction(DevTools::draw);
    ImGuiHook::setToggleCallback([this]() -> void {
        this->m_bVisible ^= 1;
    });
    ImGuiHook::setupHooks([](void* target, void* hook, void** trampoline) -> void {
        BGDInternalPlugin::get()->addHookInternal(target, hook, trampoline);
    });
    ImGuiHook::setInitFunction([]() -> void {
        ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Font_OpenSans, sizeof Font_OpenSans, 18.f);
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

void DevTools::loadStyle() {
    if (this->m_bLoadedStyle) {
        return;
    }
    
    this->m_bLoadedStyle = true;

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
        colors[ImGuiCol_Border]            = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_BorderShadow]      = colors[ImGuiCol_WindowBg];
    } else {
        colors[ImGuiCol_Border]            = ImVec4(0.71f, 0.71f, 0.71f, 0.08f);
        colors[ImGuiCol_BorderShadow]      = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
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
}

void DevTools::reloadStyle() {
    this->m_bLoadedStyle = false;
    this->loadStyle();
}

void DevTools::recurseUpdateList(CCNode* node, unsigned int i) {
    std::stringstream stream;
    stream << "[" << i << "] " << getNodeName(node);
    if (node->getTag() != -1)
        stream << " (" << node->getObjType() << " | " << node->getTag() << ")";
    else
        stream << " (" << node->getObjType() << ")";
    const auto childrenCount = node->getChildrenCount();
    if (childrenCount)
        stream << " {" << childrenCount << "}";

    auto visible = node->isVisible();

    bool main = ImGui::TreeNode(node, stream.str().c_str());
    bool hovered = ImGui::IsItemHovered();
    bool attributes = main && ImGui::TreeNode(node + 1, "Attributes");
    hovered |= main && ImGui::IsItemHovered();
    if (attributes || hovered) {
        auto& foreground = *ImGui::GetForegroundDrawList();
        auto parent = node->getParent();
        auto anchorPoint = node->getAnchorPoint();
        auto boundingBox = node->boundingBox();
        auto bbMin = CCPoint(boundingBox.getMinX(), boundingBox.getMinY());
        auto bbMax = CCPoint(boundingBox.getMaxX(), boundingBox.getMaxY());

        auto cameraParent = node;
        while (cameraParent) {
            auto camera = cameraParent->getCamera();

            auto offsetX = 0.f;
            auto offsetY = 0.f;
            auto offsetZ = 0.f;
            camera->getEyeXYZ(&offsetX, &offsetY, &offsetZ);
            auto offset = CCPoint(offsetX, offsetY);
            // they don't have -= for some reason
            bbMin = bbMin - offset;
            bbMax = bbMax - offset;

            cameraParent = cameraParent->getParent();
        }

        auto min = toVec2(parent ? parent->convertToWorldSpace(bbMin) : bbMin);
        auto max = toVec2(parent ? parent->convertToWorldSpace(bbMax) : bbMax);
        foreground.AddRectFilled(min, max, hovered ? 0x709999FF : 0x70FFFFFF);
    }

    if (main) {
        if (attributes) {
            if (ImGui::Button("Delete")) {
                node->removeFromParentAndCleanup(true);
                ImGui::TreePop();
                ImGui::TreePop();
                return;
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Child")) {
                ImGui::OpenPopup("Add Child");
            }

            if (ImGui::BeginPopupModal("Add Child")) {
                static int item = 0;
                ImGui::Combo("Node", &item, "CCNode\0CCLabelBMFont\0CCLabelTTF\0CCSprite\0CCMenuItemSpriteExtra\0");

                static int tag = -1;
                ImGui::InputInt("Tag", &tag);

                static char text[256];
                static char labelFont[256] = "bigFont.fnt";
                if (item == 1) {
                    ImGui::InputText("Text", text, 256);
                    ImGui::InputText("Font", labelFont, 256);
                }
                static int fontSize = 20;
                if (item == 2) {
                    ImGui::InputText("Text", text, 256);
                    ImGui::InputInt("Font Size", &fontSize);
                }
                static bool frame = false;
                if (item == 3 || item == 4) {
                    ImGui::InputText("Texture", text, 256);
                    ImGui::Checkbox("Frame", &frame);
                }

                ImGui::Separator();

                if (ImGui::Button("Add")) {
                    CCNode* _child = nullptr;
                    switch (item) {
                    case 0:
                        _child = CCNode::create();
                        break;
                    case 1: {
                        auto child = CCLabelBMFont::create(text, labelFont);
                        _child = child;
                        break;
                    }
                    case 2: {
                        auto child = CCLabelTTF::create(text, "Arial", static_cast<float>(fontSize));
                        _child = child;
                        break;
                    }
                    case 3: {
                        CCSprite* child;
                        if (frame)
                            child = CCSprite::createWithSpriteFrameName(text);
                        else
                            child = CCSprite::create(text);
                        _child = child;
                        break;
                    }
                    case 4: {
                        CCSprite* sprite;
                        if (frame)
                            sprite = CCSprite::createWithSpriteFrameName(text);
                        else
                            sprite = CCSprite::create(text);
                        // _child = CCMenuItemSpriteExtra::create(sprite, sprite, nullptr, nullptr);
                        break;
                    }
                    }
                    if (_child != nullptr) {
                        _child->setTag(tag);
                        node->addChild(_child);
                    }

                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::Text("Addr: 0x%p", node);
            ImGui::SameLine();
            if (ImGui::Button("Copy")) {
                std::stringstream stream;
                stream << std::uppercase << std::hex << reinterpret_cast<uintptr_t>(node);
                bgd::copyToClipboard(stream.str());
            }
            if (node->getUserData()) {
                ImGui::Text("User data: 0x%p", node->getUserData());
            }

            auto pos = node->getPosition();
            float _pos[2] = { pos.x, pos.y };
            ImGui::DragFloat2("Position", _pos);
            node->setPosition({ _pos[0], _pos[1] });

            float _scale[3] = { node->getScale(), node->getScaleX(), node->getScaleY() };
            ImGui::DragFloat3("Scale", _scale, .025f);
            // amazing
            if (node->getScale() != _scale[0])
                node->setScale(_scale[0]);
            else {
                node->setScaleX(_scale[1]);
                node->setScaleY(_scale[2]);
            }

            float _rot[3] = { node->getRotation(), node->getRotationX(), node->getRotationY() };
            ImGui::DragFloat3("Rotation", _rot);
            if (node->getRotation() != _rot[0])
                node->setRotation(_rot[0]);
            else {
                node->setRotationX(_rot[1]);
                node->setRotationY(_rot[2]);
            }

            float _skew[2] = { node->getSkewX(), node->getSkewY() };
            ImGui::DragFloat2("Skew", _skew);
            node->setSkewX(_skew[0]);
            node->setSkewY(_skew[1]);

            auto anchor = node->getAnchorPoint();
            ImGui::DragFloat2("Anchor Point", &anchor.x, 0.05f, 0.f, 1.f);
            node->setAnchorPoint(anchor);

            auto contentSize = node->getContentSize();
            ImGui::DragFloat2("Content Size", &contentSize.width);
            if (contentSize != node->getContentSize())
                node->setContentSize(contentSize);

            int zOrder = node->getZOrder();
            ImGui::InputInt("Z", &zOrder);
            if (node->getZOrder() != zOrder)
                node->setZOrder(zOrder);
            
            ImGui::Checkbox("Visible", &visible);
            if (visible != node->isVisible())
                node->setVisible(visible);

            if (dynamic_cast<CCRGBAProtocol*>(node) != nullptr) {
                auto rgbaNode = dynamic_cast<CCRGBAProtocol*>(node);
                auto color = rgbaNode->getColor();
                float _color[4] = { color.r / 255.f, color.g / 255.f, color.b / 255.f, rgbaNode->getOpacity() / 255.f };
                ImGui::ColorEdit4("Color", _color);
                rgbaNode->setColor({
                    static_cast<GLubyte>(_color[0] * 255),
                    static_cast<GLubyte>(_color[1] * 255),
                    static_cast<GLubyte>(_color[2] * 255)
                });
                rgbaNode->setOpacity(static_cast<GLubyte>(_color[3] * 255));
            }
            if (dynamic_cast<CCLabelProtocol*>(node) != nullptr) {
                auto labelNode = dynamic_cast<CCLabelProtocol*>(node);
                auto labelStr = labelNode->getString();
                char text[256];
                strcpy_s(text, labelStr);
                ImGui::InputText("Text", text, 256);
                if (strcmp(text, labelStr)) {
                    labelNode->setString(text);
                }
            }

            ImGui::TreePop();
        }
        
        auto children = node->getChildren();
        for (unsigned int i = 0; i < node->getChildrenCount(); ++i) {
            auto child = children->objectAtIndex(i);
            this->recurseUpdateList(dynamic_cast<CCNode*>(child), i);
        }
        ImGui::TreePop();
    }
}

void DevTools::generateContent() {
    static int selected_dir = 0;
    ImGui::PushItemWidth(120.f);
    if (ImGui::Combo("", &selected_dir, "Dock to Left\0Dock to Right\0Dock to Top\0Dock to Bottom\0Pop-out\0")) {
        if (selected_dir == 4) {
            this->updateVisibility(kDevToolsModePopup);
        } else {
            this->updateVisibility(kDevToolsModeIntegrated, static_cast<DevToolsMount>(selected_dir));
        }
    }
    ImGui::Separator();
    if (ImGui::BeginTabBar("dev.tabs")) {
        if (ImGui::BeginTabItem("Tree")) {
            this->recurseUpdateList(CCDirector::sharedDirector()->getRunningScene());
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Console")) {
            for (auto const& log : BGDInternal::get()->getLogs()) {
                ImGui::TextWrapped(log.toString().c_str());
                ImGui::Separator();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
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
    this->reloadStyle();
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
