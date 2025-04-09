
#include"../types.h"
#include <chrono>
#include "ProfilerObject.h"
#include"profiler.h"#include "ProfilerManager.h"

using namespace std;

namespace gpp {

ProfilerObject::ProfilerObject(const std::string& name, ProfilerManager* gm)
    : name(name), handle(gm), tstart(0), tend(0){
if(handle==nullptr){
handle=gpp_profiler::profilerInstance.get();
}

if(handle!=nullptr){
handle->pushObject(this);
}

        tstart = chrono::duration_cast<chrono::nanoseconds>(                      chrono::high_resolution_clock::now().time_since_epoch())                      .count();
}

ProfilerObject::~ProfilerObject() {

        tend = chrono::duration_cast<chrono::nanoseconds>(                    chrono::high_resolution_clock::now().time_since_epoch())                    .count();
        if (handle != nullptr) {
            int64 duration = tend - tstart;
            handle->registreProfile(this, name, duration);
        }
}

void ProfilerObject::release() {
    if (handle != nullptr) {
        handle->removeObject(this); // Remove o objeto do ProfilerManager.
        handle = nullptr;           // Evita referências pendentes.
    }
}
} // namespace gpp
