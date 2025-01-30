
#ifndef LOGBASE_H
#define LOGBASE_H

#include <functional>
#include <string>
#include <cstdint>
#include<format>
#include "LogItem.h"

namespace gpp{
enum LOGLEVEL {
    LG_ALL = 0,
    LG_DEBUG,
    LG_ERROR,
    LG_FATAL,
    LG_INFO,
    LG_WARNING
};

class LogItem;
class LogBase {
private:
    LogItem* logItem; // Agora usando um ponteiro bruto
    LOGLEVEL currentLevel;

    // Fun��o auxiliar para obter o timestamp atual
    static int64_t getCurrentTimestamp();

    // Fun��o interna para formata��o
    template<class ...Args>
    std::string formatMessage(const std::string& msg, const Args&... args) {
        return std::vformat(msg, std::make_format_args(args...));
    }

public:
    LogBase(LogItem* item, LOGLEVEL level = LG_ALL);
    ~LogBase();

    void setLevel(LOGLEVEL lv);
    LOGLEVEL getLevel() const;

    // Fun��es de conveni�ncia para diferentes n�veis de log
    template<class ...Args>
    void info(const std::string& msg, const Args&... args) {
        log(LG_INFO, msg, args...);
    }

    template<class ...Args>
    void debug(const std::string& msg, const Args&... args) {
        log(LG_DEBUG, msg, args...);
    }

    template<class ...Args>
    void error(const std::string& msg, const Args&... args) {
        log(LG_ERROR, msg, args...);
    }

    template<class ...Args>
    void fatal(const std::string& msg, const Args&... args) {
        log(LG_FATAL, msg, args...);
    }

    template<class ...Args>
    void warning(const std::string& msg, const Args&... args) {
        log(LG_WARNING, msg, args...);
    }

private:
    // Fun��o gen�rica para logar qualquer n�vel
    template<class ...Args>
    void log(LOGLEVEL level, const std::string& msg, const Args&... args) {
        if (level >= currentLevel && logItem && logItem->canLog()) {
            std::string formattedMessage = formatMessage(msg, args...);
            int64_t timestamp = getCurrentTimestamp();
            logItem->dumpMessage(level, timestamp, formattedMessage);
        }
    }
};
}
#endif // LOGBASE_H
