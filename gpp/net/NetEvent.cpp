
#include<sstream>
#include"../types.h"
#include"NetCodes.h"
#include "NetEvent.h"

using namespace std;

namespace gpp {

// Construtor padrão
NetEvent::NetEvent()
    : type(    GNET_NONE), id(0), data("") {}

// Construtor com parâmetros
NetEvent::NetEvent(uint32 type, uint32 id, const std::string& data)
    : type(type), id(id), data(data) {}

// Construtor de cópia
NetEvent::NetEvent(const NetEvent& nt)
    : type(nt.type), id(nt.id), data(nt.data) {}

// Operador de atribuição
NetEvent& NetEvent::operator=(const NetEvent& nt) {
    if (this != &nt) {
        this->type = nt.type;
        this->id = nt.id;
        this->data = nt.data;
    }
    return *this;
}

string NetEvent::toString()const{
stringstream ss;
ss<<"Id="<<id<<", Type="<<type<<endl;
ss<<"Data=\""<<data<<"\"";
return ss.str();
}

// Função de limpeza (se precisar fazer algo específico no futuro)
void NetEvent::cleanup() {
id=0;
type= GNET_NONE;
data.clear();
}
}  // namespace gpp
