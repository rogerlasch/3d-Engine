
#include<sstream>
#include"../types.h"
#include"NetCodes.h"
#include "NetEvent.h"

using namespace std;

namespace gpp {

// Construtor padr�o
NetEvent::NetEvent()
    : type(    GNET_NONE), id(0), data("") {}

// Construtor com par�metros
NetEvent::NetEvent(uint32 type, uint32 id, const std::string& data)
    : type(type), id(id), data(data) {}

// Construtor de c�pia
NetEvent::NetEvent(const NetEvent& nt)
    : type(nt.type), id(nt.id), data(nt.data) {}

// Operador de atribui��o
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

// Fun��o de limpeza (se precisar fazer algo espec�fico no futuro)
void NetEvent::cleanup() {
id=0;
type= GNET_NONE;
data.clear();
}
}  // namespace gpp
