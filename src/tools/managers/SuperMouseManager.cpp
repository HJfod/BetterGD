#include <SuperMouseManager.hpp>

USE_BGD_NAMESPACE();

void SuperMouseDelegate::mouseEnterSuper(CCPoint const&) {}
void SuperMouseDelegate::mouseLeaveSuper(CCPoint const&) {}
bool SuperMouseDelegate::mouseDownSuper(MouseButton, CCPoint const&) { return false; }
bool SuperMouseDelegate::mouseUpSuper(MouseButton, CCPoint const&) { return false; }
void SuperMouseDelegate::mouseMoveSuper(CCPoint const&) {}
void SuperMouseDelegate::mouseDownOutsideSuper(MouseButton, CCPoint const&) {}
bool SuperMouseDelegate::mouseScrollSuper(float, float) { return false; }
void SuperMouseDelegate::mouseScrollOutsideSuper(float, float) {}
void SuperMouseDelegate::setSuperMouseHitSize(CCSize const& size) {
    this->m_obSuperMouseHitSize = size;
}
void SuperMouseDelegate::setSuperMouseHitOffset(CCPoint const& pos) {
    this->m_obSuperMouseHitOffset = pos;
}
void SuperMouseDelegate::superMousePushSelf() {
    SuperMouseManager::get()->pushDelegate(this);
}
void SuperMouseDelegate::superMousePopSelf() {
    SuperMouseManager::get()->popDelegate(this);
}

SuperMouseDelegate::SuperMouseDelegate() {
    SuperMouseManager::get()->pushDelegate(this);
}

SuperMouseDelegate::~SuperMouseDelegate() {
    SuperMouseManager::get()->popDelegate(this);
}


void getTouchChildren(CCArray* arr, CCNode* node) {
    if (!node) return;
    CCARRAY_FOREACH_B_TYPE(node->getChildren(), obj, CCNode) {
        if (dynamic_cast<CCTouchDelegate*>(obj)) {
            arr->addObject(obj);
        } else if (obj->getChildrenCount()) {
            getTouchChildren(arr, obj);
        }
    }
}


bool SuperMouseManager::init() {
    return true;
}

SuperMouseManager::SuperMouseManager() {
    this->init();
}

SuperMouseManager::~SuperMouseManager() {
    for (auto const& delegate : this->m_vDelegates) {
        this->popDelegate(delegate);
    }
}

SuperMouseManager* SuperMouseManager::get() {
    static auto g_manager = new SuperMouseManager;
    return g_manager;
}

void SuperMouseManager::pushDelegate(SuperMouseDelegate* delegate) {
    this->m_vDelegates.insert(this->m_vDelegates.begin(), delegate);
}

void SuperMouseManager::popDelegate(SuperMouseDelegate* delegate) {
    this->m_vDelegates.erase(std::remove(
        this->m_vDelegates.begin(), this->m_vDelegates.end(), delegate
    ), this->m_vDelegates.end());
}

bool SuperMouseManager::delegateIsHovered(SuperMouseDelegate* delegate, CCPoint const& mpos) {
    auto size = delegate->m_obSuperMouseHitSize;

    auto p = dynamic_cast<CCNode*>(delegate);

    if (p && size == CCSizeZero)
        size = p->getScaledContentSize();

    if (p && !nodeIsVisible(p))
        return false;

    auto pos = p ? p->getPosition() : CCPointZero;

    if (p && p->getParent())
        pos = p->getParent()->convertToWorldSpace(p->getPosition());
    
    pos = pos + delegate->m_obSuperMouseHitOffset;

    auto rect = CCRect {
        pos.x - size.width / 2,
        pos.y - size.height / 2,
        size.width,
        size.height
    };

    return rect.containsPoint(mpos);
}

bool SuperMouseManager::dispatchClickEvent(MouseButton btn, bool down, CCPoint const& pos) {
    if (down) {
        this->m_vPressedButtons.insert(btn);
    } else {
        this->m_vPressedButtons.erase(btn);
    }
    if (m_pCapturing) {
        m_pCapturing->m_bSuperMouseDown = down;
        if (down)
            return m_pCapturing->mouseDownSuper(btn, pos);
        else
            return m_pCapturing->mouseUpSuper(btn, pos);
    }
    for (auto const& d : m_vDelegates) {
        if (!down) d->m_bSuperMouseDown = false;
        if (delegateIsHovered(d, pos)) {
            if (down) {
                d->m_bSuperMouseDown = true;
                if (d->mouseDownSuper(btn, pos))
                    return true;
            } else {
                if (d->mouseUpSuper(btn, pos))
                    return true;
            }
        } else {
            if (down)
                d->mouseDownOutsideSuper(btn, pos);
        }
        if (d == this->m_pWeakCapture) break;
    }
    return false;
}

bool SuperMouseManager::dispatchScrollEvent(float y, float x, CCPoint const& pos) {
    if (m_pCapturing) {
        return m_pCapturing->mouseScrollSuper(y, x);
    }
    for (auto const& d : m_vDelegates) {
        if (delegateIsHovered(d, pos)) {
            if (d->mouseScrollSuper(y, x))
                return true;
        } else
            d->mouseScrollOutsideSuper(y, x);

        if (d == this->m_pWeakCapture) break;
    }
    return false;
}

void SuperMouseManager::dispatchMoveEvent(CCPoint const& pos) {
    if (m_pCapturing) {
        this->m_obLastPosition = pos;
        return m_pCapturing->mouseMoveSuper(pos);
    }
    if (this->m_obLastPosition != pos) {
        for (auto const& d : m_vDelegates) {
            auto hover = this->delegateIsHovered(d, pos);

            if (d->m_bSuperMouseHovered != hover) {
                d->m_bSuperMouseHovered = hover;

                if (hover) {
                    if (this->m_vPressedButtons.size()) {
                        d->m_bSuperMouseDown = true;
                    }
                    d->mouseEnterSuper(pos);
                } else {
                    if (d->m_bSuperMouseDown) {
                        d->m_bSuperMouseDown = false;
                    }
                    d->mouseLeaveSuper(pos);
                }
            }

            if (hover) {
                d->mouseMoveSuper(pos);
            }

            if (d == this->m_pWeakCapture) break;
        }
    }
    this->m_obLastPosition = pos;
}

void SuperMouseManager::captureMouse(SuperMouseDelegate* delegate, bool weak) {
    if (!weak && !m_pCapturing) {
        m_pCapturing = delegate;
    }
    if (weak && !m_pWeakCapture) {
        this->m_pWeakCapture = delegate;
    }
}

void SuperMouseManager::releaseCapture(SuperMouseDelegate* delegate) {
    if (m_pCapturing == delegate)
        m_pCapturing = nullptr;
    if (m_pWeakCapture == delegate)
        m_pWeakCapture = nullptr;
}

void SuperMouseManager::releaseCapture() {
    this->m_pCapturing = nullptr;
    this->m_pWeakCapture = nullptr;
}

bool SuperMouseManager::isButtonPressed(MouseButton btn) const {
    return this->m_vPressedButtons.count(btn);
}
