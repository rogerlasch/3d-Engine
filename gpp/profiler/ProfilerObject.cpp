
#include"../types.h"
#include <chrono>
#include "ProfilerObject.h"
#include "ProfilerManager.h"

using namespace std;

namespace gpp {

ProfilerObject::ProfilerObject(const std::string& name, ProfilerManager* gm)
    : name(name), handle(gm), tstart(0), tend(0), active(false) {}

ProfilerObject::~ProfilerObject() {
    if (active) {
        stop(); // Garante que o cronômetro seja interrompido.
    }
    release(); // Libera o objeto do ProfilerManager.
}

void ProfilerObject::start() {
    if (!active) {
        active = true;
        tstart = chrono::duration_cast<chrono::nanoseconds>(                      chrono::high_resolution_clock::now().time_since_epoch())                      .count();     }
}

void ProfilerObject::stop() {
    if (active) {
        tend = chrono::duration_cast<chrono::nanoseconds>(                    chrono::high_resolution_clock::now().time_since_epoch())                    .count();
        active = false;

        if (handle != nullptr) {
            int64 duration = tend - tstart;
            handle->registreProfile(name, duration);
        }
    }
}

void ProfilerObject::release() {
    if (handle != nullptr) {
        handle->removeObject(this); // Remove o objeto do ProfilerManager.
        handle = nullptr;           // Evita referências pendentes.
    }
}
} // namespace gpp
