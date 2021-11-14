#pragma once

#define BGD_INCLUDE_IMGUI
#include "DevTools.hpp"
#include <BGDInternal.hpp>
#include <imgui/imgui_internal.h>
#include "../config.h"
#include "FeatherIcons.hpp"

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
    if (ImGui::TreeNodeEx(
        node,
        (this->m_pSelectedNode == node ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None),
        "(%d) %s", i, getNodeName(node)
    )) {
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            if (this->m_pSelectedNode == node) {
                this->m_pSelectedNode = nullptr;
            } else {
                this->m_pSelectedNode = node;
            }
        }
        CCARRAY_FOREACH_B_BASE(node->getChildren(), child, CCNode*, ix) {
            this->recurseUpdateList(dynamic_cast<CCNode*>(child), ix);
        }
        ImGui::TreePop();
    }
}

void DevTools::recurseUpdateListOdd(CCNode* node, unsigned int i) {
    std::stringstream stream;

    stream << "<" << getNodeName(node);
    if (dynamic_cast<CCScene*>(node)) {
        stream << " type=" << node->getObjType();
    }
    if (node->getTag() != -1) {
        stream << " tag=" << node->getTag();
    }
    if (node->getPositionX() != 0.f) {
        stream << " x=" << node->getPositionX();
    }
    if (node->getPositionY() != 0.f) {
        stream << " y=" << node->getPositionY();
    }
    if (node->getScale() != 1.f) {
        stream << " scale=" << node->getScale();
    }
    stream << ">";

    if (ImGui::TreeNode(node, stream.str().c_str())) {
        CCARRAY_FOREACH_B_BASE(node->getChildren(), child, CCNode*, ix) {
            this->recurseUpdateListOdd(dynamic_cast<CCNode*>(child), ix);
        }
        ImGui::TreePop();
        ImGui::Text("</%s>", getNodeName(node));
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

void DevTools::generateTree() {
    if (this->m_bOddHtmlStyleSetting) {
        this->recurseUpdateListOdd(CCDirector::sharedDirector()->getRunningScene());
    } else {
        this->recurseUpdateList(CCDirector::sharedDirector()->getRunningScene());
    }
}

void DevTools::logMessage(BGDLogMessage* log) {
    ImU32 color = 0;
    if (log->getSeverity() == kBGDSeverityWarning) {
        color = ImGui::ColorConvertFloat4ToU32(*this->m_pColorWarning);
        color = (color & 0x00ffffff) + 0x27000000; // set alpha
    }
    if (log->getSeverity() >= kBGDSeverityError) {
        color = ImGui::ColorConvertFloat4ToU32(*this->m_pColorNo);
        color = (color & 0x00ffffff) + 0x27000000; // set alpha
    }
    ImGui::BeginGroup();
    {
        ImGui::PushFont(this->m_pSmallFont);
        ImGui::Text(log->getTimeString().c_str());
        ImGui::PopFont();
        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        ImGui::PushStyleColor(ImGuiCol_Button, *this->m_pColorNo);
        if (ImGui::Button((FEATHER_X "##log.delete=" + log->toString()).c_str())) {
            BGDLoader::get()->deleteLog(log);
            return;
        }
        ImGui::PopStyleColor();
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
        ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), color
    );
    ImGui::PopFont();
    ImGui::Separator();
}

void DevTools::generateTabs() {
    this->m_vDockInfo = {};

    if (!ImGui::DockBuilderGetNode(this->m_nDockSpaceID)) {
        // this->m_nDockSpaceID = ImGui::GetID("dev.dockspace");
        // ImGui::DockBuilderRemoveNode(this->m_nDockSpaceID);
        // ImGui::DockBuilderAddNode(this->m_nDockSpaceID,
        //     ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode
        // );
    }

    if (ImGui::Begin(FEATHER_GIT_MERGE " Tree")) {
        this->generateTree();
        if (ImGui::IsWindowDocked()) {
            this->m_vDockInfo.push_back({
                ImGui::GetWindowPos().x,
                ImGui::GetWindowPos().y,
                ImGui::GetWindowWidth(),
                ImGui::GetWindowHeight()
            });
        }
    }
    ImGui::End();

    if (ImGui::Begin(FEATHER_TERMINAL  " Console")) {
        if (ImGui::BeginChild(
            0xB00B, { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 80 }, true,
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

        if (ImGui::IsWindowDocked()) {
            this->m_vDockInfo.push_back({
                ImGui::GetWindowPos().x,
                ImGui::GetWindowPos().y,
                ImGui::GetWindowWidth(),
                ImGui::GetWindowHeight()
            });
        }
    }
    ImGui::End();

    if (ImGui::Begin(FEATHER_PACKAGE   " Plugins")) {
        this->generatePluginInfo(BGDInternalPlugin::get());
        for (auto const& plugin : BGDLoader::get()->getAllPlugins()) {
            this->generatePluginInfo(plugin);
        }
        if (ImGui::IsWindowDocked()) {
            this->m_vDockInfo.push_back({
                ImGui::GetWindowPos().x,
                ImGui::GetWindowPos().y,
                ImGui::GetWindowWidth(),
                ImGui::GetWindowHeight()
            });
        }
    }
    ImGui::End();

    if (ImGui::Begin(FEATHER_CPU       " ReClass")) {
        ImGui::Text("todo");
        if (ImGui::IsWindowDocked()) {
            this->m_vDockInfo.push_back({
                ImGui::GetWindowPos().x,
                ImGui::GetWindowPos().y,
                ImGui::GetWindowWidth(),
                ImGui::GetWindowHeight()
            });
        }
    }
    ImGui::End();

    if (ImGui::Begin(FEATHER_SETTINGS  " Settings")) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.f, 0.f });
        ImGui::Checkbox("GD in Window",             &this->m_bGDInWindow);
        ImGui::Checkbox("Attributes in Node Tree",  &this->m_bAttributesInTree);
        ImGui::Checkbox("Weird XML Node Tree",      &this->m_bOddHtmlStyleSetting);
        ImGui::Checkbox("Dock With Shift",          &ImGui::GetIO().ConfigDockingWithShift);
        ImGui::PopStyleVar();

        ImGui::Separator();

        static int selected_theme = this->m_eTheme;
        ImGui::PushItemWidth((ImGui::GetWindowWidth() - 10.f) / 2);
        if (ImGui::Combo("##dev.theme", &selected_theme,
            FEATHER_SUN      " Light Theme\0"
            FEATHER_UMBRELLA " Dark Theme\0"
        )) {
            this->m_eTheme = static_cast<DevToolsTheme>(selected_theme);
            this->reloadStyle();
        }

        ImGui::Separator();

        ImGui::TextWrapped("Running " BGD_PROJECT_NAME " version " BGD_VERSION " (" BGD_VERSION_SUFFIX ")");

        if (ImGui::IsWindowDocked()) {
            this->m_vDockInfo.push_back({
                ImGui::GetWindowPos().x,
                ImGui::GetWindowPos().y,
                ImGui::GetWindowWidth(),
                ImGui::GetWindowHeight()
            });
        }
    }
    ImGui::End();

    if (ImGui::Begin(FEATHER_EDIT      " Attributes")) {
        if (!this->m_pSelectedNode) {
            ImGui::TextWrapped("Select a Node to Edit in the Scene or Tree");
        } else {
            ImGui::Text("Address: 0x%p", this->m_pSelectedNode);
            ImGui::SameLine();
            if (ImGui::Button(FEATHER_COPY " Copy")) {
                copyToClipboard(cstrfmt("%X", as<uintptr_t>(this->m_pSelectedNode)));
            }
            if (this->m_pSelectedNode->getUserData()) {
                ImGui::Text("User data: 0x%p", this->m_pSelectedNode->getUserData());
            }

            float pos[2] = {
                this->m_pSelectedNode->getPositionX(),
                this->m_pSelectedNode->getPositionY()
            };
            ImGui::DragFloat2("Position", pos);
            this->m_pSelectedNode->setPosition(pos[0], pos[1]);

            float scale[3] = { this->m_pSelectedNode->getScale(), this->m_pSelectedNode->getScaleX(), this->m_pSelectedNode->getScaleY() };
            ImGui::DragFloat3("Scale", scale, 0.025f);
            if (this->m_pSelectedNode->getScale() != scale[0]) {
                this->m_pSelectedNode->setScale(scale[0]);
            } else {
                this->m_pSelectedNode->setScaleX(scale[1]);
                this->m_pSelectedNode->setScaleY(scale[2]);
            }

            float rot[3] = { this->m_pSelectedNode->getRotation(), this->m_pSelectedNode->getRotationX(), this->m_pSelectedNode->getRotationY() };
            ImGui::DragFloat3("Rotation", rot);
            if (this->m_pSelectedNode->getRotation() != rot[0]) {
                this->m_pSelectedNode->setRotation(rot[0]);
            } else {
                this->m_pSelectedNode->setRotationX(rot[1]);
                this->m_pSelectedNode->setRotationY(rot[2]);
            }

            float _skew[2] = { this->m_pSelectedNode->getSkewX(), this->m_pSelectedNode->getSkewY() };
            ImGui::DragFloat2("Skew", _skew);
            this->m_pSelectedNode->setSkewX(_skew[0]);
            this->m_pSelectedNode->setSkewY(_skew[1]);

            auto anchor = this->m_pSelectedNode->getAnchorPoint();
            ImGui::DragFloat2("Anchor Point", &anchor.x, 0.05f, 0.f, 1.f);
            this->m_pSelectedNode->setAnchorPoint(anchor);

            auto contentSize = this->m_pSelectedNode->getContentSize();
            ImGui::DragFloat2("Content Size", &contentSize.width);
            if (contentSize != this->m_pSelectedNode->getContentSize())
                this->m_pSelectedNode->setContentSize(contentSize);

            int zOrder = this->m_pSelectedNode->getZOrder();
            ImGui::InputInt("Z", &zOrder);
            if (this->m_pSelectedNode->getZOrder() != zOrder)
                this->m_pSelectedNode->setZOrder(zOrder);
            
            auto visible = this->m_pSelectedNode->isVisible();
            ImGui::Checkbox("Visible", &visible);
            if (visible != this->m_pSelectedNode->isVisible())
                this->m_pSelectedNode->setVisible(visible);

            if (dynamic_cast<CCRGBAProtocol*>(this->m_pSelectedNode) != nullptr) {
                auto rgbaNode = dynamic_cast<CCRGBAProtocol*>(this->m_pSelectedNode);
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
            if (dynamic_cast<CCLabelProtocol*>(this->m_pSelectedNode) != nullptr) {
                auto labelNode = dynamic_cast<CCLabelProtocol*>(this->m_pSelectedNode);
                auto labelStr = labelNode->getString();
                char text[256];
                strcpy_s(text, labelStr);
                ImGui::InputText("Text", text, 256);
                if (strcmp(text, labelStr)) {
                    labelNode->setString(text);
                }
            }
        }
    }
    ImGui::End();
}
