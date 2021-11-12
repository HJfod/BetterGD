#pragma once

#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <imgui-hook.hpp>
#include "../config.h"
#include "BGDIcons.hpp"

#define CHECK_IS(var, newName, type) \
    type* newName = nullptr; if ((newName = dynamic_cast<type*>(var)))

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

void DevTools::generatePluginInfo(BGDPlugin* plugin) {
    if (ImGui::TreeNode(plugin, std::string(plugin->getName()).c_str())) {
        ImGui::TextWrapped("Name: %s",         plugin->getName());
        ImGui::TextWrapped("ID: %s",           plugin->getID());
        ImGui::TextWrapped("Description: %s",  plugin->getDescription().size() ?
                                        plugin->getDescription() :
                                        "Not provided");
        ImGui::TextWrapped("Developer: %s",    plugin->getDeveloper());
        ImGui::TextWrapped("Credits: %s",      plugin->getCredits().size() ?
                                        plugin->getCredits() :
                                        "Not provided");

        if (ImGui::TreeNode(cstrfmt("Hooks: %d", plugin->getHooks().size()))) {
            for (auto const& hook : plugin->getHooks()) {
                auto info = BGDHookInfo(hook);
                if (ImGui::TreeNode(info.formatted().c_str())) {
                    ImGui::TextWrapped(info.formattedDetails().c_str());
                    if (hook->isEnabled()) {
                        if (ImGui::Button("Disable")) {
                            plugin->disableHook(hook);
                        }
                    } else {
                        if (ImGui::Button("Enable")) {
                            plugin->enableHook(hook);
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        if (!dynamic_cast<BGDInternalPlugin*>(plugin)) {
            ImGui::Button("Reload");
            ImGui::SameLine();
            ImGui::Button("Unload");
        }

        ImGui::TreePop();
    }
}

void DevTools::logMessage(BGDLogMessage* log) {
    ImU32 color = 0;
    if (log->getSeverity() == kBGDSeverityWarning) {
        color = 0x27ff8f00;
    }
    if (log->getSeverity() >= kBGDSeverityError) {
        color = 0x27ff0000;
    }
    ImGui::BeginGroup();
    {
        ImGui::PushFont(this->m_pSmallFont);
        ImGui::Text(log->getTimeString().c_str());
        ImGui::PopFont();
        ImGui::SameLine(ImGui::GetWindowWidth() - 40);
        if (ImGui::Button(("X##log.delete=" + log->toString()).c_str())) {
            BGDLoader::get()->deleteLog(log);
            return;
        }
        auto& msgs = log->getData();
        if (!msgs.size()) {
            this->generatePluginInfo(log->getSender());
        } else {
            for (auto const& msg : msgs) {
                CHECK_IS(msg, as_str, BGDLogString) {
                    ImGui::TextWrapped(as_str->toString().c_str());
                }
                CHECK_IS(msg, as_plugin, BGDLogPlugin) {
                    this->generatePluginInfo(as_plugin->getPlugin());
                }
                CHECK_IS(msg, as_ccobj, BGDLogCCObject) {
                    auto node = dynamic_cast<CCNode*>(as_ccobj->getObject());
                    if (node) {
                        this->recurseUpdateList(node);
                    } else {
                        ImGui::Text(as_ccobj->toString().c_str());
                    }
                }
            }
        }
        ImGui::PushFont(this->m_pSmallFont);
        ImGui::Text(
            "Type: %s, Level: %s | Logged by %s",
            BGDLogTypeToString(log->getType()).c_str(),
            BGDSeverityToString(log->getSeverity()).c_str(),
            log->getSender()->getName()
        );
    }
    ImGui::EndGroup();
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetItemRectMin(), ImGui::GetItemRectMin(), color
    );
    ImGui::PopFont();
    ImGui::Separator();
}

void DevTools::generateContent() {
    static int selected_dir = 0;
    ImGui::PushItemWidth(120.f);
    if (ImGui::Combo("##dev.dock", &selected_dir,
        BGD_ICON_DOCKWEST  "Dock to Left\0"
        BGD_ICON_DOCKEAST  "Dock to Right\0"
        BGD_ICON_DOCKNORTH "Dock to Top\0"
        BGD_ICON_DOCKSOUTH "Dock to Bottom\0"
        BGD_ICON_DOCKPOP   "Pop-out\0"
    )) {
        if (selected_dir == 4) {
            this->updateVisibility(kDevToolsModePopup);
        } else {
            this->updateVisibility(kDevToolsModeIntegrated, static_cast<DevToolsMount>(selected_dir));
        }
    }
    ImGui::SameLine();
    static int selected_theme = kDevToolsThemeDark;
    ImGui::PushItemWidth(120.f);
    if (ImGui::Combo("##dev.theme", &selected_theme, "Light Theme\0Dark Theme\0")) {
        this->m_eTheme = static_cast<DevToolsTheme>(selected_theme);
        this->reloadStyle();
    }
    if (ImGui::BeginTabBar("dev.tabs", ImGuiTabBarFlags_Reorderable)) {
        if (ImGui::BeginTabItem("Tree")) {
            this->recurseUpdateList(CCDirector::sharedDirector()->getRunningScene());
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Console")) {
            if (ImGui::BeginChild(
                0xB00B, { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 180 }, true,
                ImGuiWindowFlags_HorizontalScrollbar
            )) {
                for (auto const& log : BGDLoader::get()->getLogs()) {
                    this->logMessage(log);
                }
            }
            ImGui::EndChild();
            ImGui::Separator();
            static char command_buf[255] = { 0 };
            if (this->m_bCommandSuccess) {
                memset(command_buf, 0, sizeof command_buf);
                this->m_bCommandSuccess = false;
            }
            ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 60);
            if (ImGui::InputText(
                "##dev.run_command", command_buf, IM_ARRAYSIZE(command_buf),
                ImGuiInputTextFlags_EnterReturnsTrue
            )) {
                this->executeConsoleCommand(command_buf);
                ImGui::SetKeyboardFocusHere();
            }
            ImGui::SameLine(ImGui::GetWindowWidth() - 50);
            if (ImGui::Button("Run")) {
                this->executeConsoleCommand(command_buf);
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Plugins")) {
            this->generatePluginInfo(BGDInternalPlugin::get());
            for (auto const& plugin : BGDLoader::get()->getAllPlugins()) {
                this->generatePluginInfo(plugin);
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ReClass")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings")) {
            ImGui::Checkbox("Hide Scene Overflow",      &this->m_bHideOverflow);
            ImGui::Checkbox("Attributes in Node Tree",  &this->m_bAttributesInTree);
            ImGui::Separator();
            ImGui::TextWrapped("Running " BGD_PROJECT_NAME " version " BGD_VERSION " (" BGD_VERSION_SUFFIX ")");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
