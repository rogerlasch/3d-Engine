

#include <sstream>
#include "../types.h"
#include "Event.h"

using namespace std;

namespace gpp {

Event::Event() {
    cleanup();
}

Event::~Event() {}

string Event::toString() const {
    stringstream ss;
    ss << "Type=" << type << ", V1=" << v1 << endl;
    ss << "V2=" << v2 << ", V3=" << v3 << endl; // Correção no "V3"
    ss << "Timestamp=" << timestamp << ", Data=\"" << data << "\"" << endl;
    return ss.str();
}

void Event::cleanup() {
    type = 0;
    v1 = 0;
    v2 = 0;
    v3 = 0;
    timestamp = 0;
    data.clear(); // Usar clear() em vez de atribuir uma string vazia
}
} // namespace gpp
