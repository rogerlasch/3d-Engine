
#include <chrono>
#include <stdexcept>
#include"LogItem.h"
#include "LogBase.h"

namespace gpp{
// Construtor
LogBase::LogBase(LogItem* item, LOGLEVEL level)
    : logItem(item), currentLevel(level) {
    if (!logItem) {
        throw std::invalid_argument("LogItem cannot be null");
    }
    logItem->init();
}

// Destrutor
LogBase::~LogBase() {
    if (logItem) {
        logItem->end();
    }
}

// Define o nível de log atual
void LogBase::setLevel(LOGLEVEL lv) {
    currentLevel = lv;
}

// Retorna o nível de log atual
LOGLEVEL LogBase::getLevel() const {
    return currentLevel;
}

// Função auxiliar para obter o timestamp atual
int64_t LogBase::getCurrentTimestamp() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
}
