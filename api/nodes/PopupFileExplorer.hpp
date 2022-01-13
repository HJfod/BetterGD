#pragma once

#include <BGDUtils.hpp>
#include "BrownAlertDelegate.hpp"

namespace bgd {
    typedef void (cocos2d::CCObject::*SEL_FileHandler)(std::filesystem::path const&);
    typedef void (cocos2d::CCObject::*SEL_MultiFileHandler)(std::vector<std::filesystem::path> const&);

    #define file_selector(_SELECTOR) (SEL_FileHandler)(&_SELECTOR)
    #define multifile_selector(_SELECTOR) (SEL_MultiFileHandler)(&_SELECTOR)

    using SelectFileFunc        = std::function<void(std::filesystem::path const&)>;
    using SelectMultiFileFunc   = std::function<void(std::vector<std::filesystem::path> const&)>;

    class PopupFileExplorer : public BrownAlertDelegate {
        protected:
            std::filesystem::path m_sPath;
            cocos2d::CCObject* m_pSelTarget                  = nullptr;
            SEL_FileHandler m_pSelFile             = nullptr;
            SEL_MultiFileHandler m_pSelFileMulti   = nullptr;
            SelectFileFunc m_pFileCallback          = nullptr;
            SelectMultiFileFunc m_pMultiCallback    = nullptr;

            void setup() override;

            void onBackFolder(cocos2d::CCObject*);
            void onEnterFolder(cocos2d::CCObject*);
            void onSelectFile(cocos2d::CCObject*);
            void onRefresh(cocos2d::CCObject*);

        public:
            static PopupFileExplorer* create();
            static PopupFileExplorer* create(std::string const& path);
            static PopupFileExplorer* create(std::filesystem::path const& path);

            PopupFileExplorer* setSelector(cocos2d::CCObject* target, SEL_FileHandler selector);
            PopupFileExplorer* setSelector(cocos2d::CCObject* target, SEL_MultiFileHandler selector);
            PopupFileExplorer* setCallback(SelectFileFunc);
            PopupFileExplorer* setCallback(SelectMultiFileFunc);
    };
}
