#include <BGDSaveManager.hpp>
#include <BGDPlugin.hpp>
#include <utils/other/ext.hpp>

USE_BGD_NAMESPACE();

void bgd::BGDSaveManager::firstSetup() {}
void bgd::BGDSaveManager::saveDict(DS_Dictionary*) {}
void bgd::BGDSaveManager::loadDict(DS_Dictionary*) {}
void bgd::BGDSaveManager::saveJSON(nlohmann::json&) {}
void bgd::BGDSaveManager::loadJSON(nlohmann::json&) {}
void bgd::BGDSaveManager::saveCustom(std::string const&) {}
void bgd::BGDSaveManager::loadCustom(std::string const&) {}

void bgd::BGDSaveManager::setup(BGDPlugin* plugin) {
    this->m_pOwner = plugin;
    if (this->m_pOwner) {
        this->m_pOwner->registerSaveManager(this);
    }
}

bgd::BGDSaveManager::BGDSaveManager() {}
bgd::BGDSaveManager::~BGDSaveManager() {
    if (this->m_pOwner) {
        this->m_pOwner->unregisterSaveManager(this);
    }
}

void bgd::BGDSaveManager::saveData(std::string const& dir) {
    std::string path = dir + "/" + this->m_sSaveDirectory + "/" + this->m_sSaveFileName;

    switch (this->m_eSaveType) {
        case kBGDSaveTypeCustom: default: {
            this->saveCustom(path);
        } break;
            
        case kBGDSaveTypeJSON: {
            nlohmann::json json;
            this->saveJSON(json);
            file_write_string(path, json.dump(4));
        } break;
        
        case kBGDSaveTypeDSDictionary: {
            auto dict = new DS_Dictionary;
            this->saveDict(dict);
            dict->saveRootSubDictToCompressedFile(path.c_str());
            delete dict;
        } break;
    }
}

void bgd::BGDSaveManager::loadData(std::string const& dir) {
    std::string path = dir + "/" + this->m_sSaveDirectory + "/" + this->m_sSaveFileName;

    if (!std::filesystem::exists(path))
        return;

    switch (this->m_eSaveType) {
        case kBGDSaveTypeCustom: default: {
            this->loadCustom(path);
        } break;
            
        case kBGDSaveTypeJSON: {
            auto res = file_read_string(path);
            if (res) {
                nlohmann::json json = nlohmann::json::parse(res.value());
                this->loadJSON(json);
            }
        } break;
        
        case kBGDSaveTypeDSDictionary: {
            auto dict = new DS_Dictionary;
            if (dict->loadRootSubDictFromCompressedFile(path.c_str())) {
                this->loadDict(dict);
            }
            delete dict;
        } break;
    }
}

