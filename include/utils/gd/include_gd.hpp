#pragma once

#define GDH_IS_DLL

// not feeling like making GD.h dll-interface,
// so just disable the warnings ig
#pragma warning(disable: 4275)  // inheritance
#pragma warning(disable: 4251)  // members
#pragma warning(disable: 4067)  // shut up about unexpected tokens in cocos2d

#include "../../bgd/BGDMacros.hpp"

#include <cocos2d.h>
#include <gd.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

#pragma warning(default: 4067)

#define USE_BGD_NAMESPACE()         \
using namespace gd;                 \
using namespace bgd;                \
using namespace cocos2d;            \
using namespace cocos2d::extension; \
