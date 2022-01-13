#pragma once

#include <BGDUtils.hpp>
#include "BrownAlertDelegate.hpp"
#include "InputNode.hpp"

namespace bgd {
    class BGD_DLL InputPrompt : public BrownAlertDelegate {
        public:
            using InputResult = std::function<void(const char*)>;
        protected:
            InputNode* m_pInput;
            InputResult m_pResultFunc;
            CCMenuItemSpriteExtra* m_pTrashBtn;
            bool m_bApplyOnEsc = false;

            inline void setup() override {};
            void onApply(cocos2d::CCObject*);
            void onTrash(cocos2d::CCObject*);
            void onClose(cocos2d::CCObject*) override;
            bool init(const char* title, const char* inputText, InputResult resFunc, const char* applyText);

        public:
            static InputPrompt* create(
                const char* title,
                const char* inputText,
                InputResult resFunc,
                const char* applyText = "Apply"
            );

            inline InputPrompt* setApplyOnEsc(bool on) {
                this->m_bApplyOnEsc = on; return this;
            }

            inline InputPrompt* setTrashButton(bool on) {
                this->m_pTrashBtn->setVisible(on); return this;
            }

            inline InputPrompt* setCallback(InputResult resFunc) {
                this->m_pResultFunc = resFunc; return this;
            }

            inline InputPrompt* setFilter(std::string const& filter) {
                this->m_pInput->getInputNode()->setAllowedChars(filter);
                return this;
            }

            inline InputNode* getInputNode() {
                return m_pInput;
            }
    };
}
