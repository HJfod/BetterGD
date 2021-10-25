#pragma once

#include "include_gd.hpp"

namespace bgd {
    BGD_DLL void limitNodeSize(
        cocos2d::CCNode* node,
        cocos2d::CCSize const& size,
        float def,
        float min
    );

    template<typename ... Args>
    const char* cstrfmt(const char* fmt, Args ... args) {
        return cocos2d::CCString::createWithFormat(fmt, args ... )->getCString();
    }
}
