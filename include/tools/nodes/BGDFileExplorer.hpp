#pragma once

#include <BGDUtils.hpp>
#include "BrownAlertDelegate.hpp"

namespace bgd {
    typedef void (CCObject::*SEL_FileHandler)(std::filesystem::path const&);
    typedef void (CCObject::*SEL_MultiFileHandler)(std::vector<std::filesystem::path> const&);

    #define file_selector(_SELECTOR) (SEL_FileHandler)(&_SELECTOR)
    #define multifile_selector(_SELECTOR) (SEL_MultiFileHandler)(&_SELECTOR)

    using SelectFileFunc        = std::function<void(std::filesystem::path const&)>;
    using SelectMultiFileFunc   = std::function<void(std::vector<std::filesystem::path> const&)>;

    class BGDFileExplorer : public BrownAlertDelegate {
        protected:
            std::filesystem::path m_sPath;
            CCObject* m_pSelTarget                  = nullptr;
            SEL_FileHandler* m_pSelFile             = nullptr;
            SEL_MultiFileHandler* m_pSelFileMulti   = nullptr;
            SelectFileFunc m_pFileCallback          = nullptr;
            SelectMultiFileFunc m_pMultiCallback    = nullptr;

            void setup() override;

            void onBackFolder(CCObject*);
            void onEnterFolder(CCObject*);
            void onSelectFile(CCObject*);
            void onRefresh(CCObject*);

        public:
            static BGDFileExplorer* create();
            static BGDFileExplorer* create(std::string const& path);
            static BGDFileExplorer* create(std::filesystem::path const& path);

            BGDFileExplorer* setSelector(CCObject* target, SEL_FileHandler selector);
            BGDFileExplorer* setSelector(CCObject* target, SEL_MultiFileHandler selector);
            BGDFileExplorer* setCallback(SelectFileFunc);
            BGDFileExplorer* setCallback(SelectMultiFileFunc);
    };
}
