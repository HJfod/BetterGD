#pragma once

#include "include_gd.hpp"

namespace bgd {
    BGD_DLL void limitNodeSize(
        cocos2d::CCNode* node,
        cocos2d::CCSize const& size,
        float def,
        float min
    );
}
