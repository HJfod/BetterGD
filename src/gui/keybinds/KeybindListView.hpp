#pragma once

#include <BetterGD.hpp>
#include <tools/keybinds/KeybindManager.hpp>

USE_BGD_NAMESPACE();

static constexpr const BoomListType kBoomListType_Keybind
    = static_cast<BoomListType>(0x421);

class CustomKeybindingsLayer;

struct KeybindItem : public CCObject {
    KeybindAction* m_pAction = nullptr;
    std::string m_sText;
    bool m_bSelectMode = false;
    Keybind m_obSelectKeybind;

    KeybindItem(
        KeybindAction* action,
        bool selectMode = false,
        Keybind const& kb = Keybind()
    ) :
        m_pAction(action),
        m_bSelectMode(selectMode),
        m_obSelectKeybind(kb) {}

    KeybindItem(std::string const& text) :
        m_sText(text) {}
};

class KeybindCell : public TableViewCell, public FLAlertLayerProtocol {
    protected:
        CustomKeybindingsLayer* m_pKeybindingsLayer;
        KeybindItem* m_pItem;
        CCMenu* m_pMenu;

		KeybindCell(const char* name, CCSize size);

        void FLAlert_Clicked(FLAlertLayer*, bool) override;

        friend class KeybindEditPopup;
        friend class KeybindRepeatPopup;
        friend class MoreKeybindsPopup;

    public:
        void loadFromItem(KeybindItem* bind);
        void updateBGColor(int index);
        void updateMenu();
        void onEdit(CCObject*);
        void onRepeat(CCObject*);
        void onFold(CCObject*);
        void onReset(CCObject*);
        void onSelect(CCObject*);
        void onMore(CCObject*);
        void onDescription(CCObject*);

		static KeybindCell* create(const char* key, CCSize size);
};

class KeybindListView : public CustomListView {
    protected:
        CustomKeybindingsLayer* m_pKeybindingsLayer;

        void setupList() override;
        TableViewCell* getListCell(const char* key) override;
        void loadCell(TableViewCell* cell, unsigned int index) override;

    public:
        static KeybindListView* create(CCArray* binds, float width, float height);
};
