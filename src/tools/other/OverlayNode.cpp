#include <tools/other/OverlayNode.hpp>

USE_BGD_NAMESPACE();

bool OverlayNode::init() {
    if (!CCNode::init())
        return false;
    
    this->moveCurrentNoficiationNode();

    return true;
}

void OverlayNode::moveCurrentNoficiationNode() {
    if (CCDirector::sharedDirector()->getNotificationNode() != this) {
        auto node = CCDirector::sharedDirector()->getNotificationNode();
        this->addChild(node);
        CCDirector::sharedDirector()->setNotificationNode(this);
    }
}

OverlayNode* OverlayNode::create() {
    auto res = new OverlayNode;
    if (res && res->init()) {
        res->autorelease();
        return res;
    }
    CC_SAFE_DELETE(res);
    return nullptr;
}

OverlayNode* OverlayNode::get() {
    static auto OverlayNode* g_node = OverlayNode::create();
    g_node->moveCurrentNoficiationNode();
    return g_node;
}
