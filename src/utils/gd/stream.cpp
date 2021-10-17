#include <utils/gd/stream.hpp>

std::ostream& bgd::operator<<(std::ostream& stream, cocos2d::CCPoint const& pos) {
    return stream << pos.x << ", " << pos.y;
}

std::ostream& bgd::operator<<(std::ostream& stream, cocos2d::CCSize const& size) {
    return stream << size.width << " : " << size.height;
}

std::ostream& bgd::operator<<(std::ostream& stream, cocos2d::CCRect const& rect) {
    return stream << rect.origin << " | " << rect.size;
}
