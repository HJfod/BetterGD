#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

enum DevToolsMode {
    kDevToolsModeIntegrated,
    kDevToolsModePopup,
};

enum DevToolsMount {
    kDevToolsMountWest,
    kDevToolsMountEast,
    kDevToolsMountNorth,
    kDevToolsMountSouth,
};

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
        bool m_bHideOverflow    = true;
        bool m_bAttributesInTree= false;
        bool m_bCommandSuccess  = false;
        bool m_bOddHtmlStyleSetting=false;
        float m_fWidth          = 200.f;
        float m_fMinWidth       = 100.f;
        float m_fMaxWidth       = 700.f;
        float m_fHeight         = 100.f;
        float m_fMinHeight      = 50.f;
        float m_fMaxHeight      = 500.f;
        float m_fPadding        = 20.f;
        CCSize m_obPopoutSize   = { 500.f, 500.f };
        DevToolsMode  m_eMode   = kDevToolsModeIntegrated;
        DevToolsMount m_eMount  = kDevToolsMountWest;
        DevToolsTheme m_eTheme  = kDevToolsThemeDark;
        ImFont* m_pDefaultFont  = nullptr;
        ImFont* m_pSmallFont    = nullptr;
        ImVec4* m_pColorNo      = nullptr;
        ImVec4* m_pColorYes     = nullptr;
        ImVec4* m_pColorWarning = nullptr;

        void showAnimation(CCScene*, bool transition);
        void loadStyle();
        void reloadStyle();
        void recurseUpdateList(CCNode* parent, unsigned int = 0);
        void recurseUpdateListOdd(CCNode* parent, unsigned int = 0);
        void generateTree();
        void generatePluginInfo(BGDPlugin* plugin);
        void generateContent();
        float getSceneScale();
        void resizeWindow();
        void updateVisibility(DevToolsMode, DevToolsMount = kDevToolsMountWest);
        void logMessage(BGDLogMessage* msg);
        void loadTheme(DevToolsTheme theme);
        void hideOverflow();
        void executeConsoleCommand(std::string const&);

        void draw();

        DevTools();
        ~DevTools();

    public:
        static DevTools* get();

        void fixSceneScale(CCScene* scene);
        void willSwitchToScene(CCScene* scene);

        void show();
        void hide();
        void toggle();
};
