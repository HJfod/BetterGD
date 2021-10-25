#pragma once

#include "include_gd.hpp"

namespace bgd {
    void ccDrawColor4B(cocos2d::ccColor4B const& color);
    cocos2d::ccColor4B invert4B(cocos2d::ccColor4B const& color);
    cocos2d::ccColor3B invert3B(cocos2d::ccColor3B const& color);
    cocos2d::ccColor3B to3B(cocos2d::ccColor4B const& color);
    cocos2d::ccColor4B to4B(cocos2d::ccColor3B const& color);
    cocos2d::ccColor4F to4F(cocos2d::ccColor4B const& color);
    constexpr cocos2d::ccColor3B cc3x(int hexValue);

    template<typename T>
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec) {
        auto res = cocos2d::CCArray::create();
        for (auto const& item : vec)
            res->addObject(item);
        return res;
    }

    template<typename T, class T2>
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec, std::function<T2(T)> convFunc) {
        auto res = cocos2d::CCArray::create();
        for (auto const& item : vec)
            res->addObject(convFunc(item));
        return res;
    }
}
