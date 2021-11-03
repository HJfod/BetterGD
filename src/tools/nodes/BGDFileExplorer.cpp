#include <BGDFileExplorer.hpp>

USE_BGD_NAMESPACE();

std::filesystem::path g_path;

void BGDFileExplorer::setup() {
}

void BGDFileExplorer::onBackFolder(CCObject*) {
    this->m_sPath = this->m_sPath.parent_path();
    this->onRefresh(nullptr);
}

void BGDFileExplorer::onEnterFolder(CCObject* pSender) {
    auto folder = as<CCString*>(as<CCNode*>(pSender)->getUserObject())->getCString();
    this->m_sPath /= folder;
    this->onRefresh(nullptr);
}

void BGDFileExplorer::onSelectFile(CCObject* pSender) {
    auto file = as<CCString*>(as<CCNode*>(pSender)->getUserObject())->getCString();
    auto path = this->m_sPath / file;
    if (this->m_pTarget && this->m_pSelFile) {
        (this->m_pTarget->*m_pSelFile)(this, path);
    }
    if (this->m_pTarget && this->m_pSelFileMulti) {
        (this->m_pTarget->*m_pSelFileMulti)(this, { path });
    }
    if (this->m_pFileCallback) {
        this->m_pFileCallback(path);
    }
    if (this->m_pMultiCallback) {
        this->m_pMultiCallback({ path });
    }
}

BGDFileExplorer* BGDFileExplorer::setSelector(CCObject* target, SEL_FileHandler selector) {
    this->m_pTarget = target;
    this->m_pSelFile = selector;
    return this;
}

BGDFileExplorer* BGDFileExplorer::setSelector(CCObject* target, SEL_MultiFileHandler selector) {
    this->m_pTarget = target;
    this->m_pSelFileMulti = selector;
    return this;
}

BGDFileExplorer* BGDFileExplorer::setCallback(SelectFileFunc func) {
    this->m_pFileCallback = func;
    return this;
}

BGDFileExplorer* BGDFileExplorer::setCallback(SelectMultiFileFunc func) {
    this->m_pMultiCallback = func;
    return this;
}

BGDFileExplorer* BGDFileExplorer::create() {
    return this->create(g_path);
}

BGDFileExplorer* BGDFileExplorer::create(std::string const& path) {
    return this->create(std::filesystem::path(path));
}

BGDFileExplorer* BGDFileExplorer::create(std::filesystem::path const& path) {
    auto ret = new BGDFileExplorer;
    if (ret && (ret->m_sPath = path || true) && ret->init(
        420.f, 280.f
    )) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
