#pragma once

#include "include_gd.hpp"

std::ostream& operator<<(std::ostream& stream, cocos2d::CCPoint const& pos);
std::ostream& operator<<(std::ostream& stream, cocos2d::CCSize  const& size);
std::ostream& operator<<(std::ostream& stream, cocos2d::CCRect  const& rect);
