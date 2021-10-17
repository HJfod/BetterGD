#pragma once

#include "include_gd.hpp"

// todo: define all of these ._.

cocos2d::CCPoint operator*=(cocos2d::CCPoint & pos, float mul);
cocos2d::CCSize  operator*=(cocos2d::CCSize  & size, float mul);
cocos2d::CCRect  operator*=(cocos2d::CCRect  & size, float mul);
cocos2d::CCPoint operator/=(cocos2d::CCPoint & pos, float div);
cocos2d::CCSize  operator/=(cocos2d::CCSize  & pos, float div);
cocos2d::CCRect  operator/=(cocos2d::CCRect  & pos, float div);
cocos2d::CCPoint operator+=(cocos2d::CCPoint & pos,  cocos2d::CCPoint const& add);
cocos2d::CCSize  operator+=(cocos2d::CCSize  & size, cocos2d::CCPoint const& add);
cocos2d::CCSize  operator+=(cocos2d::CCSize  & size, cocos2d::CCSize  const& add);
cocos2d::CCRect  operator+=(cocos2d::CCRect  & rect, cocos2d::CCPoint const& add);
cocos2d::CCRect  operator+=(cocos2d::CCRect  & rect, cocos2d::CCSize  const& add);
cocos2d::CCRect  operator+=(cocos2d::CCRect  & rect, cocos2d::CCRect  const& add);
cocos2d::CCSize  operator- (cocos2d::CCSize const& size, float f);
cocos2d::CCSize  operator- (cocos2d::CCSize const& size);
bool operator==(cocos2d::CCSize  const& s1,   cocos2d::CCSize  const& s2);
bool operator!=(cocos2d::CCSize  const& s1,   cocos2d::CCSize  const& s2);
bool operator==(cocos2d::CCPoint const& p1,   cocos2d::CCPoint const& p2);
bool operator!=(cocos2d::CCPoint const& p1,   cocos2d::CCPoint const& p2);
bool operator==(cocos2d::CCRect  const& r1,   cocos2d::CCRect  const& r2);
bool operator!=(cocos2d::CCRect  const& r1,   cocos2d::CCRect  const& r2);
