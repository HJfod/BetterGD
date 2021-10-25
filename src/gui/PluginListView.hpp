#pragma once

#include <BetterGD.hpp>

USE_BGD_NAMESPACE();

static constexpr const BoomListType kBoomListType_Plugin
    = static_cast<BoomListType>(0x350);

struct PluginObject : public CCObject {
    BGDPlugin* m_pPlugin;

    inline PluginObject(BGDPlugin* plugin) : m_pPlugin(plugin) {
        this->autorelease();
    };
};

class PluginCell : public TableViewCell {
    protected:
        BGDPlugin* m_pPlugin;

		PluginCell(const char* name, CCSize size);

        void draw() override;
	
	public:
        void loadFromPlugin(PluginObject*);

		static PluginCell* create(const char* key, CCSize size);
};

class PluginListView : public CustomListView {
    protected:
        void setupList() override;
        TableViewCell* getListCell(const char* key) override;
        void loadCell(TableViewCell* cell, unsigned int index) override;
    
    public:
        static PluginListView* create(
            CCArray* actions,
            float width, float height
        );
};
