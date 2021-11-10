#include <BGDLog.hpp>
#include <BGDInternal.hpp>

USE_BGD_NAMESPACE();

BGDLog::~BGDLog() {}

BGDLogCCObject::~BGDLogCCObject() {
    if (this->m_pObj) {
        this->m_pObj->release();
    }
}

BGDLogMany::~BGDLogMany() {
    for (auto const& log : this->m_vLogs) {
        delete log;
    }
}

std::string BGDLogError::toString(bool logTime) const {
    std::stringstream res;

    if (this->m_pPlugin) {
        std::cout << this->m_pPlugin->getName() << "\n";
        res << std::string(this->m_pPlugin->getName()) << ": ";
    }
    res << BGDSeverityToString(this->m_eSeverity);
    if (this->m_eSeverity != kBGDSeverityError) {
        res << " Error ";
    }
    if (logTime) {
        res << " at " << timePointAsString(this->m_obTime);
    }
    res << ": " << this->m_sDescription;

    return res.str();
}

std::string BGDLogError::toShortString() const {
    return BGDSeverityToString(this->m_eSeverity) + ": "_s + this->m_sDescription;
}

std::string BGDLogMessage::toString(bool logTime) const {
    std::stringstream res;

    if (this->m_pPlugin) {
        res << this->m_pPlugin->getName() << ": ";
    }
    if (logTime) {
        res << " at " << timePointAsString(this->m_obTime);
    }
    res << ": " << this->m_sMessage;

    return res.str();
}

std::string BGDLogMessage::toShortString() const {
    return this->m_sMessage;
}

std::string BGDLogPlugin::toString(bool logTime) const {
    std::stringstream res;

    if (this->m_pPlugin) {
        res << " { " << this->m_pPlugin->getName() << " } ";
    }
    if (logTime) {
        res << " at " << timePointAsString(this->m_obTime);
    }

    return res.str();
}

std::string BGDLogPlugin::toShortString() const {
    return std::string(this->m_pPlugin->getName());
}

std::string BGDLogCCObject::toString(bool logTime) const {
    std::stringstream res;

    if (this->m_pPlugin) {
        res << this->m_pPlugin->getName() << ": ";
    }
    if (logTime) {
        res << " at " << timePointAsString(this->m_obTime);
    }
    res << ": " << getNodeName(this->m_pObj);

    return res.str();
}

std::string BGDLogCCObject::toShortString() const {
    return "["_s + getNodeName(this->m_pObj) + "]"_s;
}

std::string BGDLogMany::toString(bool logTime) const {
    std::stringstream res;

    if (this->m_pPlugin) {
        res << this->m_pPlugin->getName() << ": ";
    }
    if (logTime) {
        res << " at " << timePointAsString(this->m_obTime);
    }
    for (auto const& log : this->m_vLogs) {
        res << " " << log->toShortString();
    }

    return res.str();
}

std::string BGDLogMany::toShortString() const {
    return this->toString();
}

void BGDLogStream::init() {
    if (!this->m_pLog) {
        this->m_pLog = new BGDLogMessage;
    }
}

BGDLogStream& BGDLogStream::operator<<(bgd::BGDPlugin* plugin) {
    if (!this->m_pLog) {
        this->m_pLog = new BGDLogPlugin(plugin);
    } else if (!this->m_pLog->m_pPlugin) {
        this->m_pLog->m_pPlugin = plugin;
    } else if (dynamic_cast<BGDLogMany*>(this->m_pLog)) {
        dynamic_cast<BGDLogMany*>(this->m_pLog)->add(new BGDLogPlugin(plugin));
    } else {
        auto old = this->m_pLog;
        this->m_pLog = new BGDLogMany({ old, new BGDLogPlugin(plugin) }, old->m_pPlugin);
    }
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCObject* obj) {
    if (!this->m_pLog) {
        this->m_pLog = new BGDLogCCObject(obj, nullptr);
    } else if (dynamic_cast<BGDLogPlugin*>(this->m_pLog)) {
        auto old = this->m_pLog;
        this->m_pLog = new BGDLogCCObject(obj, old->m_pPlugin);
        delete old;
    } else if (dynamic_cast<BGDLogMany*>(this->m_pLog)) {
        dynamic_cast<BGDLogMany*>(this->m_pLog)->add(new BGDLogCCObject(obj, nullptr));
    } else {
        auto old = this->m_pLog;
        this->m_pLog = new BGDLogMany({ old, new BGDLogCCObject(obj, old->m_pPlugin) }, old->m_pPlugin);
    }
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(std::string const& str) {
    this->init();
    this->m_sStream << str;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(const char* str) {
    this->init();
    this->m_sStream << str;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(int n) {
    this->init();
    this->m_sStream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(long n) {
    this->init();
    this->m_sStream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(float n) {
    this->init();
    this->m_sStream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(double n) {
    this->init();
    this->m_sStream << n;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCPoint const& pos) {
    this->init();
    this->m_sStream << pos.x << ", " << pos.y;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCSize const& size) {
    this->init();
    this->m_sStream << size.width << " : " << size.height;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCRect const& rect) {
    this->init();
    this->m_sStream << rect.origin << " | " << rect.size;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(bgd::endl_type) {
    if (!this->m_pLog) {
        this->m_pLog = new BGDLogMessage(this->m_sStream.str(), nullptr);
    } else if (dynamic_cast<BGDLogMessage*>(this->m_pLog)) {
        dynamic_cast<BGDLogMessage*>(this->m_pLog)->m_sMessage = this->m_sStream.str();
    } else if (dynamic_cast<BGDLogMany*>(this->m_pLog)) {
        dynamic_cast<BGDLogMany*>(this->m_pLog)->add(new BGDLogMessage(this->m_sStream.str(), nullptr));
    } else {
        auto old = this->m_pLog;
        this->m_pLog = new BGDLogMany({
            old,
            new BGDLogMessage(this->m_sStream.str(), old->m_pPlugin)
        }, old->m_pPlugin);
    }

    BGDLoader::get()->log(this->m_pLog);

    BGD_PLATFORM_CONSOLE(
        std::cout << this->m_pLog->toString(true) << "\n";
    )

    // BGDLoader manages this memory now
    this->m_pLog = nullptr;
    this->m_sStream.str(std::string());

    return *this;
}

BGDLogStream::~BGDLogStream() {
    if (this->m_pLog) {
        delete this->m_pLog;
    }
}
