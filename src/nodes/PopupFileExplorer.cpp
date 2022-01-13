#include <PopupFileExplorer.hpp>

USE_BGD_NAMESPACE();

std::filesystem::path g_path;

void PopupFileExplorer::setup() {
}

void PopupFileExplorer::onRefresh(CCObject*) {

}

void PopupFileExplorer::onBackFolder(CCObject*) {
    this->m_sPath = this->m_sPath.parent_path();
    this->onRefresh(nullptr);
}

void PopupFileExplorer::onEnterFolder(CCObject* pSender) {
    auto folder = as<CCString*>(as<CCNode*>(pSender)->getUserObject())->getCString();
    this->m_sPath /= folder;
    this->onRefresh(nullptr);
}

void PopupFileExplorer::onSelectFile(CCObject* pSender) {
    auto file = as<CCString*>(as<CCNode*>(pSender)->getUserObject())->getCString();
    auto path = this->m_sPath / file;
    if (this->m_pTarget && this->m_pSelFile) {
        (this->m_pTarget->*m_pSelFile)(path);
    }
    if (this->m_pTarget && this->m_pSelFileMulti) {
        (this->m_pTarget->*m_pSelFileMulti)({ path });
    }
    if (this->m_pFileCallback) {
        this->m_pFileCallback(path);
    }
    if (this->m_pMultiCallback) {
        this->m_pMultiCallback({ path });
    }
}

PopupFileExplorer* PopupFileExplorer::setSelector(CCObject* target, SEL_FileHandler selector) {
    this->m_pTarget = target;
    this->m_pSelFile = selector;
    return this;
}

PopupFileExplorer* PopupFileExplorer::setSelector(CCObject* target, SEL_MultiFileHandler selector) {
    this->m_pTarget = target;
    this->m_pSelFileMulti = selector;
    return this;
}

PopupFileExplorer* PopupFileExplorer::setCallback(SelectFileFunc func) {
    this->m_pFileCallback = func;
    return this;
}

PopupFileExplorer* PopupFileExplorer::setCallback(SelectMultiFileFunc func) {
    this->m_pMultiCallback = func;
    return this;
}

PopupFileExplorer* PopupFileExplorer::create() {
    return PopupFileExplorer::create(g_path);
}

PopupFileExplorer* PopupFileExplorer::create(std::string const& path) {
    return PopupFileExplorer::create(std::filesystem::path(path));
}

PopupFileExplorer* PopupFileExplorer::create(std::filesystem::path const& path) {
    auto ret = new PopupFileExplorer;
    if (ret) {
        ret->m_sPath = path;
        if (ret->init(
            420.f, 280.f
        )) {
            ret->autorelease();
            return ret;
        }
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
