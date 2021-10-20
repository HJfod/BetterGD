#pragma once

#include <BGDUtils.hpp>

class BGD_DLL MonoSpaceLabel : public cocos2d::CCLabelBMFont {
    protected:
        static inline float kMonoSpaceLabelDefaultCharWidth = 24.0f;
        static inline float kMonoSpaceLabelDefaultLineHeight = 30.0f;
        static inline cocos2d::CCTextAlignment kMonoSpaceLabelDefaultTextAlignment = cocos2d::kCCTextAlignmentLeft;

        float m_fCharWidth = kMonoSpaceLabelDefaultCharWidth;
        float m_fLineHeight = kMonoSpaceLabelDefaultLineHeight;

        bool init(
            const char* text,
            const char* bmFile,
            float charWidth,
            cocos2d::CCTextAlignment alignment,
            float labelWidth
        );

        void updateLabel() override;
    public:
        static MonoSpaceLabel* create(
            const char* text,
            const char* bmFile
        );
        static MonoSpaceLabel* create(
            const char* text,
            const char* bmFile,
            float charWidth
        );
        static MonoSpaceLabel* createWithWidth(
            const char* text,
            const char* bmFile,
            float labelWidth
        );
        static MonoSpaceLabel* create(
            const char* text,
            const char* bmFile,
            float charWidth,
            float labelWidth
        );
        static MonoSpaceLabel* create(
            const char* text,
            const char* bmFile,
            float charWidth,
            cocos2d::CCTextAlignment alignment
        );
        static MonoSpaceLabel* create(
            const char* text,
            const char* bmFile,
            float charWidth,
            cocos2d::CCTextAlignment alignment,
            float labelWidth
        );

        void setCharWidth(float);
        float getCharWidth();

        void setLineHeight(float);
        float getLineHeight();
};
