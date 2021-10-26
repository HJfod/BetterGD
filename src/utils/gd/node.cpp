#include <utils/gd/node.hpp>

USE_BGD_NAMESPACE();

bool bgd::nodeIsVisible(CCNode* t) {
    if (!t->isVisible())
        return false;

    if (t->getParent())
        return nodeIsVisible(t->getParent());
    
    return true;
}

bool bgd::nodeIsHovered(CCNode* node, CCPoint const& gpos) {
    auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
    auto size = node->getScaledContentSize();

    auto rect = CCRect {
        pos.x - size.width / 2,
        pos.y - size.height / 2,
        size.width,
        size.height
    };

    return rect.containsPoint(gpos);
}

CCPoint bgd::getMousePos() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto winSizePx = CCDirector::sharedDirector()->getOpenGLView()->getViewPortRect();
    auto ratio_w = winSize.width / winSizePx.size.width;
    auto ratio_h = winSize.height / winSizePx.size.height;
    auto mpos = CCDirector::sharedDirector()->getOpenGLView()->getMousePosition();
    mpos.y = winSizePx.size.height - mpos.y;
    mpos.x *= ratio_w;
    mpos.y *= ratio_h;
    return mpos;
}
