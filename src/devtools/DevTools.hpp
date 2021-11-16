#pragma once

#include <BGDUtils.hpp>
#ifdef BGD_INCLUDE_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include "RTTI/address.hpp"
#else
struct ImFont;
struct ImVec4;
using ImGuiID = unsigned int;
#endif

USE_BGD_NAMESPACE();

enum DevToolsTheme {
    kDevToolsThemeLight,
    kDevToolsThemeDark,
};

class DevTools {
    protected:
        bool m_bVisible         = false;
        bool m_bLoadedStyle     = false;
        bool m_bGDInWindow      = true;
        bool m_bAttributesInTree= false;
        bool m_bCommandSuccess  = false;
        size_t m_nLastLogCount  = 0;
        bool m_bOddHtmlStyleSetting=false;
        DevToolsTheme m_eTheme  = kDevToolsThemeDark;
        ImFont* m_pDefaultFont  = nullptr;
        ImFont* m_pSmallFont    = nullptr;
        ImFont* m_pMonoFont     = nullptr;
        ImFont* m_pBoxFont      = nullptr;
        ImVec4* m_pColorNo      = nullptr;
        ImVec4* m_pColorYes     = nullptr;
        ImVec4* m_pColorWarning = nullptr;
        CCNode* m_pSelectedNode = nullptr;
        ImGuiID m_nDockSpaceID  = 0;
        AddressManager* m_pAddresses = nullptr;

        void executeConsoleCommand(std::string const&);
        
        void initFonts();
        void loadStyle();
        void reloadStyle();
        void loadTheme(DevToolsTheme theme);

        void recurseUpdateList(CCNode* parent, unsigned int = 0);
        void recurseUpdateListOdd(CCNode* parent, unsigned int = 0);
        void recurseGetParents(std::vector<CCNode*>& vec, CCNode* node);
        void logMessage(BGDLogMessage* msg);
        void generatePluginInfo(BGDPlugin* plugin);
        void hoverableNodeName(CCNode* node);
        void generateTree();
        void generateTabs();
        template<int TabID>
        void generateTab();
        void addressData(uintptr_t address);
        void selectNode(CCNode* node);

        void draw();
        
        friend void CCEGLView_swapBuffers(CCEGLView*);
        friend void CCDirector_drawScene(CCDirector*);

        DevTools();
        ~DevTools();

    public:
        static DevTools* get();

        void fixSceneScale(CCScene* scene);
        void willSwitchToScene(CCScene* scene);

        bool isVisible() const;
        bool shouldPopGame() const;
        void show();
        void hide();
        void toggle();
};
