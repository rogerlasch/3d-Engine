
#include"../types.h"
#include "profiler.h"

using namespace std;

namespace gpp {

// Definição do ponteiro estático
unique_ptr<ProfilerManager> gpp_profiler::profilerInstance = nullptr;

void gpp_profiler::start(uint32 resolution, const string& filename) {
    if (!profilerInstance) {
        profilerInstance = make_unique<ProfilerManager>(resolution, filename);
    }
}

string gpp_profiler::dump_string() {
    if (profilerInstance) {
        return profilerInstance->dump();
    }
    return "";
}

void gpp_profiler::dump_to_file() {
    if (profilerInstance) {
        profilerInstance->dumpToFile();
    }
}
}
