#pragma once

#include "include_gd.hpp"

namespace bgd {
    BGD_DLL bool nodeIsVisible(cocos2d::CCNode* node);
    BGD_DLL bool nodeIsHovered(cocos2d::CCNode* node, cocos2d::CCPoint const& gpos);
    BGD_DLL cocos2d::CCPoint getMousePos();
}
