#pragma once

#include <utils/gd/include_gd.hpp>
#include <utils/other/general.hpp>

namespace bgd {
    class BGDPlugin;
    class BGDLogStream;

    using log_clock = std::chrono::system_clock;

    enum BGDSeverity {
        kBGDSeverityDebug,      // no need to even pay attention
        kBGDSeverityInfo,       // fyi :)
        kBGDSeverityNotice,     // hmm
        kBGDSeverityWarning,    // oh shit
        kBGDSeverityError,      // oh fuck
        kBGDSeverityCritical,   // oh god oh fuck
        kBGDSeverityAlert,      // panic
        kBGDSeverityEmergency,  // !!!!!!!!!!!!!
    };

    static std::string BGDSeverityToString(BGDSeverity severity) {
        switch (severity) {
            case kBGDSeverityDebug:     return "Debug";
            case kBGDSeverityInfo:      return "Info";
            case kBGDSeverityNotice:    return "Notice";
            case kBGDSeverityWarning:   return "Warning";
            case kBGDSeverityError:     return "Error";
            case kBGDSeverityCritical:  return "Critical";
            case kBGDSeverityAlert:     return "Alert";
            case kBGDSeverityEmergency: return "Emergency";
            default:                    return "Unknown";
        }
    }

    enum BGDLogType {
        kBGDLogTypeGeneral,
        kBGDLogTypeError,
        kBGDLogTypeData,
    };

    static std::string BGDLogTypeToString(BGDLogType type) {
        switch (type) {
            case kBGDLogTypeGeneral:        return "General";
            case kBGDLogTypeError:          return "Error";
            case kBGDLogTypeData:           return "Data";
            default:                        return "Unknown";
        }
    }

    class BGD_DLL BGDLog {
        protected:
            friend class BGDLogStream;

        public:
            virtual std::string toString() const = 0;

            virtual ~BGDLog();
    };

    class BGD_DLL BGDLogPlugin : public BGDLog {
        protected:
            BGDPlugin* m_pPlugin;

            friend class BGDLogStream;
        
        public:
            BGDLogPlugin(BGDPlugin* plugin) :
                m_pPlugin(plugin) {}
            
            inline BGDPlugin* getPlugin() const {
                return m_pPlugin;
            }

            std::string toString() const override;
    };

    class BGD_DLL BGDLogString : public BGDLog {
        protected:
            std::string m_sString;

            friend class BGDLogStream;

        public:
            std::string toString() const override;

            BGDLogString() = default;
            BGDLogString(
                std::string const& str
            ) : m_sString(str) {}
    };

    class BGD_DLL BGDLogCCObject : public BGDLog {
        protected:
            cocos2d::CCObject* m_pObj;
        
            friend class BGDLogStream;

        public:
            BGDLogCCObject() = default;
            BGDLogCCObject(
                cocos2d::CCObject* obj
            ) : m_pObj(obj) {
                obj->retain();
            }
            ~BGDLogCCObject();

            inline cocos2d::CCObject* getObject() { return m_pObj; }
            
            std::string toString() const override;
    };

    class BGD_DLL BGDLogMessage {
        protected:
            BGDPlugin* m_pSender            = nullptr;
            log_clock::time_point m_obTime  = log_clock::now();
            std::vector<BGDLog*> m_vData    = {};
            BGDSeverity m_eSeverity         = kBGDSeverityDebug;
            BGDLogType  m_eType             = kBGDLogTypeGeneral;

            friend class BGDLogStream;
        
        public:
            BGDLogMessage() = default;

            BGDLogMessage(
                BGDPlugin* plugin
            ) : m_pSender(plugin) {}

            BGDLogMessage(
                std::string data,
                BGDPlugin* plugin
            ) : m_vData({ new BGDLogString(data) }),
                m_pSender(plugin) {}

            BGDLogMessage(
                std::string data,
                BGDSeverity severity,
                BGDLogType type,
                BGDPlugin* plugin
            ) : m_vData({ new BGDLogString(data) }),
                m_eSeverity(severity),
                m_eType(type),
                m_pSender(plugin) {}

            BGDLogMessage(
                std::initializer_list<BGDLog*> data,
                BGDPlugin* plugin
            ) : m_pSender(plugin),
                m_vData(data) {}

            ~BGDLogMessage();

            inline void add(BGDLog* msg) {
                this->m_vData.push_back(msg);
            }

            inline log_clock::time_point getTime() const {
                return this->m_obTime;
            }
            inline std::string getTimeString() const {
                return timePointAsString(this->m_obTime);
            }
            inline BGDLogType getType() const {
                return this->m_eType;
            }
            inline BGDPlugin* getSender() const {
                return this->m_pSender;
            }
            inline BGDSeverity getSeverity() const {
                return this->m_eSeverity;
            }
            inline std::vector<BGDLog*> const& getData() const {
                return this->m_vData;
            }

            std::string toString(bool logTime = true) const;
    };

    struct endl_type {
        constexpr endl_type() {}
    };
    constexpr const auto endl = endl_type();

    class BGD_DLL BGDLogStream {
        protected:
            BGDLogMessage* m_pLog = nullptr;
            std::stringstream m_sStream;

            void init();
            void save();
            void finish();

        public:
            BGDLogStream& operator<<(BGDPlugin*);
            BGDLogStream& operator<<(void*);
            BGDLogStream& operator<<(BGDSeverity);
            BGDLogStream& operator<<(BGDLogType);
            BGDLogStream& operator<<(cocos2d::CCObject*);
            BGDLogStream& operator<<(std::string const&);
            BGDLogStream& operator<<(const char*);
            BGDLogStream& operator<<(int);
            BGDLogStream& operator<<(long);
            BGDLogStream& operator<<(float);
            BGDLogStream& operator<<(double);
            BGDLogStream& operator<<(cocos2d::CCPoint const&);
            BGDLogStream& operator<<(cocos2d::CCSize const&);
            BGDLogStream& operator<<(cocos2d::CCRect const&);
            BGDLogStream& operator<<(endl_type);

            ~BGDLogStream();
    };
}
