#include <BGDLog.hpp>
#include <BGDInternal.hpp>

USE_BGD_NAMESPACE();

BGDLog::~BGDLog() {}

BGDLogCCObject::~BGDLogCCObject() {
    if (this->m_pObj) {
        this->m_pObj->release();
    }
}

BGDLogMessage::~BGDLogMessage() {
    for (auto const& log : this->m_vData) {
        delete log;
    }
}

std::string BGDLogString::toString() const {
    return this->m_sString;
}

std::string BGDLogPlugin::toString() const {
    return "[ " + std::string(this->m_pPlugin->getName()) + " ]";
}

std::string BGDLogCCObject::toString() const {
    return "{ " + std::string(getNodeName(this->m_pObj)) + " }";
}

std::string BGDLogMessage::toString(bool logTime) const {
    std::stringstream res;

    if (this->m_pSender) {
        res << this->m_pSender->getName();
    }
    if (logTime) {
        res << " at " << timePointAsString(this->m_obTime);
    }
    res << ":";
    for (auto const& log : this->m_vData) {
        res << " " << log->toString();
    }

    return res.str();
}

void BGDLogStream::init() {
    if (!this->m_pLog) {
        this->m_pLog = new BGDLogMessage;
    }
}

void BGDLogStream::save() {
    if (this->m_pLog && this->m_sStream.str().size()) {
        this->m_pLog->add(new BGDLogString(this->m_sStream.str()));
        this->m_sStream.str(std::string());
    }
}

void BGDLogStream::finish() {
    this->init();
    this->save();

    BGDLoader::get()->log(this->m_pLog);

    BGD_PLATFORM_CONSOLE(
        std::cout << this->m_pLog->toString(true) << "\n";
    )

    // BGDLoader manages this memory now
    this->m_pLog = nullptr;
    this->m_sStream.str(std::string());
}

BGDLogStream& BGDLogStream::operator<<(bgd::BGDPlugin* plugin) {
    this->save();
    if (!this->m_pLog) {
        this->m_pLog = new BGDLogMessage(plugin);
    } else if (!this->m_pLog->m_pSender) {
        this->m_pLog->m_pSender = plugin;
    } else {
        this->m_pLog->add(new BGDLogPlugin(plugin));
    }
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(cocos2d::CCObject* obj) {
    this->save();
    this->init();
    this->m_pLog->add(new BGDLogCCObject(obj));
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(BGDSeverity severity) {
    this->init();
    this->m_pLog->m_eSeverity = severity;
    return *this;
}

BGDLogStream& BGDLogStream::operator<<(BGDLogType type) {
    this->init();
    this->m_pLog->m_eType = type;
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
    this->finish();
    return *this;
}

BGDLogStream::~BGDLogStream() {
    if (this->m_pLog) {
        delete this->m_pLog;
    }
}
