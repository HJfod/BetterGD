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

ccColor4B bgd::to4B(ccColor3B const& c, GLubyte alpha) {
    return { c.r, c.g, c.b, alpha };
}

ccColor4F bgd::to4F(ccColor4B const& c) {
    return { c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f };
}
