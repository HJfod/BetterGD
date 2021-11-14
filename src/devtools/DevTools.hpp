#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

enum DevToolsTheme {
    kDevToolsThemeLight,
    kDevToolsThemeDark,
};

struct ImFont;
struct ImVec4;

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

        void updateSceneScale(CCScene*);
        void loadStyle();
        void reloadStyle();
        void recurseUpdateList(CCNode* parent, unsigned int = 0);
        void recurseUpdateListOdd(CCNode* parent, unsigned int = 0);
        void generateTree();
        void generatePluginInfo(BGDPlugin* plugin);
        void generateTabs();
        void resizeWindow();
        void logMessage(BGDLogMessage* msg);
        void loadTheme(DevToolsTheme theme);
        void executeConsoleCommand(std::string const&);

        void draw();

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
