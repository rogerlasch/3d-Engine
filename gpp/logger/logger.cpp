
#include<sstream>
#include<stacktrace>
#include<string>
#include "logger.h"

using namespace std;

namespace gpp{
// Inicializa��o dos membros est�ticos
std::unique_ptr<LogBase> logger::globallogger = nullptr;
std::mutex logger::loggerMutex;

// M�todo est�tico para abrir o logger global
void logger::open(LogItem* item, LOGLEVEL level) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    if (!item) {
        throw std::invalid_argument("LogItem cannot be null");
    }
if(globallogger ==NULL){
    globallogger = std::make_unique<LogBase>(item, level);
}
else{
info("O log j� est� aberto, n�o � poss�vel substituir o log ap�s ele ser aberto.");
delete item;
}
}

string getStackTrace() {
ostringstream oss;

    // Captura o stack trace atual
    auto trace = std::stacktrace::current();

    // Itera pelos frames do stack trace
    for (std::size_t i = 0; i < trace.size(); ++i) {
        const auto& frame = trace[i];
        oss << "#" << i << " " << frame.description() << "\n";
    }

    return oss.str();
}
}
