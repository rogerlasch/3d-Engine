
#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <mutex>
#include<fstream>
#include <stacktrace>
#include"../types.h"
#include"LogItem.h"
#include "LogBase.h"
#include"LogFile.h"

namespace gpp{

class logger {
private:
    static std::unique_ptr<LogBase> globallogger;
    static std::mutex loggerMutex;

public:
    // Inicializa o logger global
    static void open(LogItem* item, LOGLEVEL level = LG_ALL);

    // Funções de conveniência para diferentes níveis de log
    template<class ...Args>
    static void info(const std::string& msg, const Args&... args) {
        std::lock_guard<std::mutex> lock(loggerMutex);
        if (globallogger) {
            globallogger->info(msg, args...);
        }
    }

    template<class ...Args>
    static void debug(const std::string& msg, const Args&... args) {
        std::lock_guard<std::mutex> lock(loggerMutex);
        if (globallogger) {
            globallogger->debug(msg, args...);
        }
    }

    template<class ...Args>
    static void error(const std::string& msg, const Args&... args) {
        std::lock_guard<std::mutex> lock(loggerMutex);
        if (globallogger) {
            globallogger->error(msg, args...);
        }
    }

    template<class ...Args>
    static void fatal(const std::string& msg, const Args&... args) {
        std::lock_guard<std::mutex> lock(loggerMutex);
        if (globallogger) {
            globallogger->fatal(msg, args...);
        }
    }

    template<class ...Args>
    static void warning(const std::string& msg, const Args&... args) {
        std::lock_guard<std::mutex> lock(loggerMutex);
        if (globallogger) {
            globallogger->warning(msg, args...);
        }
    }
};

std::string getStackTrace();
}
#endif // LOGGER_H
