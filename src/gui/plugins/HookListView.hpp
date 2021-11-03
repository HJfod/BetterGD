#pragma once

#include <BetterGD.hpp>

USE_BGD_NAMESPACE();

static constexpr const BoomListType kBoomListType_Hooks
    = static_cast<BoomListType>(0x358);

struct HookItem : public CCObject {
    BGDHook* m_pHook;

    HookItem(BGDHook* h) : m_pHook(h) {
        this->autorelease();
    }
};

class HookCell : public TableViewCell {
    protected:
        BGDPlugin* m_pPlugin;
        BGDHook* m_pHook;

		HookCell(const char* name, CCSize size);

        void draw() override;

        void onEnable(CCObject*);
	
	public:
        void loadFromHook(BGDHook*, BGDPlugin*);

		static HookCell* create(const char* key, CCSize size);
};

class HookListView : public CustomListView {
    protected:
        BGDPlugin* m_pPlugin;

        void setupList() override;
        TableViewCell* getListCell(const char* key) override;
        void loadCell(TableViewCell* cell, unsigned int index) override;
    
    public:
        static HookListView* create(
            CCArray* hooks, BGDPlugin* plugin, float width, float height
        );
};
