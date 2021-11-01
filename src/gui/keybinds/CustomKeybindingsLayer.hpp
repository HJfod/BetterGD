#pragma once

#include <BetterGD.hpp>
#include "KeybindListView.hpp"

USE_BGD_NAMESPACE();

class CustomKeybindingsLayer :
    public BrownAlertDelegate,
    public TextInputDelegate,
    public FLAlertLayerProtocol
{
    protected:
        KeybindListView* m_pList;
        std::string m_sSearchQuery = "";
        InputNode* m_pSearchBar;
        Scrollbar* m_pScrollbar;
        CCMenuItemSpriteExtra* m_pSelectBtn;
        BGLabel* m_pSelectLabel;

        void setup() override;

        void reloadList();

        void onKeymap(CCObject*);
        void onFinishSelect(CCObject*);
        void onGlobalSettings(CCObject*);
        void onResetAll(CCObject*);

    public:
        static CustomKeybindingsLayer* create();
};
