#include <MonoSpaceLabel.hpp>

using namespace cocos2d;
using namespace bgd;
using namespace bgd::cast;

bool MonoSpaceLabel::init(
    const char* text,
    const char* bmFile,
    float charWidth,
    CCTextAlignment alignment,
    float labelWidth
) {
    this->m_fCharWidth = charWidth;

    if (!CCLabelBMFont::initWithString(text, bmFile, labelWidth, alignment, CCPointZero))
        return false;

    return true;
}

// it would be so much better to just override createFontChars
// however, for some reason that isn't virtual
// bruh
void MonoSpaceLabel::updateLabel() {
    this->setString(m_sInitialString, false);

    if (m_fWidth) {
        float lineWidth = m_fWidth / m_fScaleX;
        float posX = 0.0f;

        CCObject* child;
        auto index = 0;
        CCARRAY_FOREACH(m_pChildren, child) {
            auto pNode = reinterpret_cast<CCNode*>(child);

            if (pNode) {
                if (m_sString[index] == '\n') {
                    posX = 0.0f;
                }

                pNode->setPositionX(posX);

                posX += m_fCharWidth;
            }
        
            index++;
        }

        this->setContentSize({ lineWidth, this->getContentSize().height });
    }

    this->CCLabelBMFont::updateLabel();
}

void MonoSpaceLabel::setCharWidth(float value) {
    this->m_fCharWidth = value;
}

float MonoSpaceLabel::getCharWidth() {
    return this->m_fCharWidth;
}

MonoSpaceLabel* MonoSpaceLabel::create(
    const char* text,
    const char* bmFile,
    float charWidth,
    cocos2d::CCTextAlignment alignment,
    float labelWidth
) {
    auto pRet = new MonoSpaceLabel;

    if (pRet && pRet->init(text, bmFile, charWidth, alignment, labelWidth)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

// overloads

MonoSpaceLabel* MonoSpaceLabel::create(
    const char* text,
    const char* bmFile,
    float charWidth,
    cocos2d::CCTextAlignment alignment
) {
    return create(text, bmFile, charWidth, alignment, kCCLabelAutomaticWidth);
}

MonoSpaceLabel* MonoSpaceLabel::create(
    const char* text,
    const char* bmFile,
    float charWidth,
    float labelWidth
) {
    return create(text, bmFile, charWidth, kMonoSpaceLabelDefaultTextAlignment, labelWidth);
}

MonoSpaceLabel* MonoSpaceLabel::create(
    const char* text,
    const char* bmFile,
    float charWidth
) {
    return create(text, bmFile, charWidth, kMonoSpaceLabelDefaultTextAlignment, kCCLabelAutomaticWidth);
}

MonoSpaceLabel* MonoSpaceLabel::createWithWidth(
    const char* text,
    const char* bmFile,
    float labelWidth
) {
    return create(text, bmFile, kMonoSpaceLabelDefaultCharWidth, kMonoSpaceLabelDefaultTextAlignment, labelWidth);
}

MonoSpaceLabel* MonoSpaceLabel::create(
    const char* text,
    const char* bmFile
) {
    return create(text, bmFile, kMonoSpaceLabelDefaultCharWidth, kMonoSpaceLabelDefaultTextAlignment, kCCLabelAutomaticWidth);
}
