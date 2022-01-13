#include <Scrollbar.hpp>
#include <BGDInternal.hpp>

USE_BGD_NAMESPACE();

bool Scrollbar::mouseDownSuper(MouseButton, cocos2d::CCPoint const& mpos) {
    if (!this->m_pList)
        return false;

    SuperMouseManager::get()->captureMouse(this);

    auto pos = this->convertToNodeSpace(mpos);

    auto h = this->m_pList->m_pTableView->m_pContentLayer->getScaledContentSize().height
        - this->m_pList->m_fHeight + this->m_pList->m_pTableView->m_fScrollLimitTop;
    auto p = this->m_pList->m_fHeight /
        this->m_pList->m_pTableView->m_pContentLayer->getScaledContentSize().height;

    auto trackHeight = min(p, 1.0f) * this->m_pList->m_fHeight / .4f;

    auto posY = h * (
        (-pos.y - this->m_pList->m_fHeight / 2 + trackHeight / 4 - 5) /
        (this->m_pList->m_fHeight - trackHeight / 2 + 10)
    );

    if (posY > 0.0f)
        posY = 0.0f;
    
    if (posY < -h)
        posY = -h;
    
    auto offsetY = this->m_pList->m_pTableView->m_pContentLayer->getPositionY() - posY;

    if (fabsf(offsetY) < trackHeight) {
        this->m_obClickOffset = CCPoint {
            pos.x,
            this->m_pList->m_pTableView->m_pContentLayer->getPositionY() - posY
        };
    } else {
        this->m_obClickOffset = CCPointZero;
    }
    
    return true;
}

bool Scrollbar::mouseUpSuper(MouseButton, cocos2d::CCPoint const&) {
    SuperMouseManager::get()->releaseCapture(this);
    return true;
}

void Scrollbar::mouseMoveSuper(cocos2d::CCPoint const& mpos) {
    if (!this->m_pList)
        return;

    if (this->m_bSuperMouseDown) {
        auto pos = this->convertToNodeSpace(mpos);

        auto h = this->m_pList->m_pTableView->m_pContentLayer->getScaledContentSize().height
            - this->m_pList->m_fHeight + this->m_pList->m_pTableView->m_fScrollLimitTop;
        auto p = this->m_pList->m_fHeight /
            this->m_pList->m_pTableView->m_pContentLayer->getScaledContentSize().height;

        auto trackHeight = min(p, 1.0f) * this->m_pList->m_fHeight / .4f;

        auto posY = h * (
            (-pos.y - this->m_pList->m_fHeight / 2 + trackHeight / 4 - 5) /
            (this->m_pList->m_fHeight - trackHeight / 2 + 10)
        );

        posY += this->m_obClickOffset.y;

        if (posY > 0.0f)
            posY = 0.0f;
        
        if (posY < -h)
            posY = -h;

        this->m_pList->m_pTableView->m_pContentLayer->setPositionY(posY);
    }
}

bool Scrollbar::mouseScrollSuper(float y, float x) {
    if (!this->m_pList)
        return false;
    this->m_pList->scrollWheel(x, y);
    return true;
}

void Scrollbar::draw() {
    CCLayer::draw();

    if (!this->m_pList) return;

    this->m_pBG->setContentSize({
        this->m_fWidth, this->m_pList->m_fHeight
    });
    this->m_pBG->setScale(1.0f);
    this->m_pBG->setColor({ 0, 0, 0 });
    this->m_pBG->setOpacity(150);
    this->m_pBG->setPosition(0.0f, 0.0f);

    this->setSuperMouseHitSize({
        this->m_fWidth, this->m_pList->m_fHeight
    });

    auto h = this->m_pList->m_pTableView->m_pContentLayer->getScaledContentSize().height
        - this->m_pList->m_fHeight + this->m_pList->m_pTableView->m_fScrollLimitTop;
    auto p = this->m_pList->m_fHeight /
        this->m_pList->m_pTableView->m_pContentLayer->getScaledContentSize().height;

    auto trackHeight = min(p, 1.0f) * this->m_pList->m_fHeight / .4f;
    auto trackPosY = this->m_pTrack->getPositionY();

    GLubyte o = 100;

    if (this->m_bSuperMouseHovered) {
        o = 160;
    }
    if (this->m_bSuperMouseDown) {
        o = 255;
    }

    this->m_pTrack->setScale(.4f);
    this->m_pTrack->setColor({ o, o, o });


    auto y = this->m_pList->m_pTableView->m_pContentLayer->getPositionY();

    trackPosY = - this->m_pList->m_fHeight / 2 + trackHeight / 4 - 5.0f + 
        ((-y) / h) * (this->m_pList->m_fHeight - trackHeight / 2 + 10.0f);

    auto fHeightTop = [&]() -> float {
        return trackPosY - this->m_pList->m_fHeight / 2 + trackHeight * .4f / 2 + 3.0f;
    };
    auto fHeightBottom = [&]() -> float {
        return trackPosY + this->m_pList->m_fHeight / 2 - trackHeight * .4f / 2 - 3.0f;
    };
    
    if (fHeightTop() > 0.0f) {
        trackHeight -= fHeightTop();
        trackPosY -= fHeightTop();
    }
    
    if (fHeightBottom() < 0.0f) {
        trackHeight += fHeightBottom();
        trackPosY -= fHeightBottom();
    }

    this->m_pTrack->setPosition(0.0f, trackPosY);

    this->m_pTrack->setContentSize({
        this->m_fWidth, trackHeight
    });
}

void Scrollbar::setList(BoomListView* list) {
    this->m_pList = list;
}

bool Scrollbar::init(BoomListView* list) {
    if (!this->CCLayer::init())
        return false;
    
    this->setList(list);
    this->m_fWidth = 8.0f;

    if (BGDInternal::isFileInSearchPaths("BGD_scrollbar.png")) {
        this->m_pBG = CCScale9Sprite::create(
            "BGD_scrollbar.png"
        );
        this->m_pTrack = CCScale9Sprite::create(
            "BGD_scrollbar.png"
        );
    } else {
        this->m_pBG = CCScale9Sprite::create(
            "square02_small.png", { 0.0f, 0.0f, 40.0f, 40.0f }
        );
        this->m_pTrack = CCScale9Sprite::create(
            "square02_small.png", { 0.0f, 0.0f, 40.0f, 40.0f }
        );
    }

    this->addChild(this->m_pBG);
    this->addChild(this->m_pTrack);

    return true;
}

Scrollbar* Scrollbar::create(BoomListView* list) {
    auto ret = new Scrollbar;

    if (ret && ret->init(list)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

