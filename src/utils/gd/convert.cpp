#include <utils/gd/convert.hpp>

USE_BGD_NAMESPACE();

void bgd::ccDrawColor4B(ccColor4B const& c) {
    cocos2d::ccDrawColor4B(c.r, c.g, c.b, c.a);
}

ccColor4B bgd::invert4B(ccColor4B const& c) {
    return {
        static_cast<GLubyte>(255 - c.r),
        static_cast<GLubyte>(255 - c.g),
        static_cast<GLubyte>(255 - c.b),
        c.a
    };
}

ccColor3B bgd::invert3B(ccColor3B const& c) {
    return {
        static_cast<GLubyte>(255 - c.r),
        static_cast<GLubyte>(255 - c.g),
        static_cast<GLubyte>(255 - c.b)
    };
}

ccColor3B bgd::to3B(ccColor4B const& c) {
    return { c.r, c.g, c.b };
}

ccColor4B bgd::to4B(ccColor3B const& c) {
    return { c.r, c.g, c.b, 255 };
}

ccColor4F bgd::to4F(ccColor4B const& c) {
    return { c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f };
}

constexpr ccColor3B bgd::cc3x(int hexValue) {
    if (hexValue <= 0xf)
        return ccColor3B {
            static_cast<GLubyte>(hexValue * 17),
            static_cast<GLubyte>(hexValue * 17),
            static_cast<GLubyte>(hexValue * 17)
        };
    if (hexValue <= 0xff)
        return ccColor3B {
            static_cast<GLubyte>(hexValue),
            static_cast<GLubyte>(hexValue),
            static_cast<GLubyte>(hexValue)
        };
    if (hexValue <= 0xfff)
        return ccColor3B {
            static_cast<GLubyte>((hexValue >> 8 & 0xf) * 17),
            static_cast<GLubyte>((hexValue >> 4 & 0xf) * 17),
            static_cast<GLubyte>((hexValue >> 0 & 0xf) * 17)
        };
    else
        return ccColor3B {
            static_cast<GLubyte>(hexValue >> 16 & 0xff),
            static_cast<GLubyte>(hexValue >> 8  & 0xff),
            static_cast<GLubyte>(hexValue >> 0  & 0xff)
        };
}

