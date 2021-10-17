#pragma once

#include "../../bgd/BGDMacros.hpp"
#include <cocos2d.h>
#include <gd.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

#define USE_BGD_NAMESPACE()         \
using namespace gd;                 \
using namespace bgd;                \
using namespace cocos2d;            \
using namespace cocos2d::extension; \
