#include <BGDLogStream.hpp>
#include <BGDInternal.hpp>

USE_BGD_NAMESPACE();

std::string BGDLogMessage::toString() const {
    std::stringstream res;

    if (this->m_error) {
        res << BGDSeverityToString(this->m_error->severity) << "! ";
    }

    if (this->m_plugin) {
        res << this->m_plugin->getName() << " ";
    }
    res << " at " << timePointAsString(this->m_time);
    if (this->m_ccobj) {
        res << " with " << getNodeName(this->m_ccobj);
    }
    if (this->m_error) {
        res << ": " << this->m_error->description;
    } else {
        res << ": " << this->m_info;
    }

    return res.str();
}

void BGDLogStream::init() {
    if (!this->m_init) {
        this->m_msg.m_time = std::chrono::system_clock::now();
        this->m_init = true;
    }
}

BGDLogStream& BGDLogStream::operator<<(bgd::BGDPlugin* plugin) {
    this->init();
    this->m_msg.m_plugin = plugin;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCObject* obj) {
    this->init();
    this->m_msg.m_ccobj = obj;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(std::string const& str) {
    this->init();
    this->m_stream << str;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(const char* str) {
    this->init();
    this->m_stream << str;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(int n) {
    this->init();
    this->m_stream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(long n) {
    this->init();
    this->m_stream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(float n) {
    this->init();
    this->m_stream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(double n) {
    this->init();
    this->m_stream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCPoint const& pos) {
    this->init();
    this->m_stream << pos.x << ", " << pos.y;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCSize const& size) {
    this->init();
    this->m_stream << size.width << " : " << size.height;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCRect const& rect) {
    this->init();
    this->m_stream << rect.origin << " | " << rect.size;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(bgd::endl_type) {
    this->m_msg.m_info = this->m_stream.str();

    BGDInternal::get()->log(this->m_msg);

    std::cout << this->m_msg.toString();

    this->m_msg = BGDLogMessage();
    this->m_stream.str(std::string());
    this->m_init = false;

    return *this;
}
