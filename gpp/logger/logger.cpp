
#include<sstream>
#include<stacktrace>
#include<string>
#include "logger.h"

using namespace std;

namespace gpp{
// Inicialização dos membros estáticos
std::unique_ptr<LogBase> logger::globallogger = nullptr;
std::mutex logger::loggerMutex;

// Método estático para abrir o logger global
void logger::open(LogItem* item, LOGLEVEL level) {
    std::lock_guard<std::mutex> lock(loggerMutex);
    if (!item) {
        throw std::invalid_argument("LogItem cannot be null");
    }
if(globallogger ==NULL){
    globallogger = std::make_unique<LogBase>(item, level);
}
else{
info("O log já está aberto, não é possível substituir o log após ele ser aberto.");
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
