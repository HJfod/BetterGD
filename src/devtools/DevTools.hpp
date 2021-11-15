#pragma once

#include <BGDUtils.hpp>
#ifdef BGD_INCLUDE_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
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
        bool m_bOddHtmlStyleSetting=false;
        DevToolsTheme m_eTheme  = kDevToolsThemeDark;
        ImFont* m_pDefaultFont  = nullptr;
        ImFont* m_pSmallFont    = nullptr;
        ImVec4* m_pColorNo      = nullptr;
        ImVec4* m_pColorYes     = nullptr;
        ImVec4* m_pColorWarning = nullptr;
        std::vector<CCRect> m_vDockInfo;
        CCNode* m_pSelectedNode = nullptr;
        ImGuiID m_nDockSpaceID  = 0;

        void updateSceneScale(CCScene*);

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
        void generateTree();
        void generateTabs();

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
