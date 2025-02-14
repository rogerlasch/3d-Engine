
#include<iostream>
#include"types.h"
#include "ErrorSystem.h"

using namespace std;

namespace gpp{
static GPP_ERRORCALLBACK internalErrorCallback = nullptr;
static GPP_ERRORCALLBACK userErrorCallback = nullptr;

string gpp_get_error_text(uint32 hcode) {
    switch (hcode) {
    case GERROR_OK: return "Nenhum erro.";
    case GERROR_UNKNOWN: return "Um erro desconhecido aconteceu.";
    case GERROR_INVALID_PARAM: return "Par�metro inv�lido.";
    case GERROR_NOTFOUND: return "Recurso n�o encontrado.";
    case GERROR_OPEN: return "Falha ao abrir o recurso.";
    case GERROR_INVALID_STATE: return "Estado inv�lido.";
    case GERROR_CONTENT: return "Conte�do inv�lido ou corrompido.";
    case GERROR_FORMAT: return "Formato inv�lido.";
    case GERROR_LARGE: return "Tamanho excessivo.";
    case GERROR_INVALID_OPERATION: return "Opera��o inv�lida.";
    case GERROR_ALREADY: return "A��o j� realizada.";
    case GERROR_EXCEPTION: return "Uma exce��o foi lan�ada.";
    default: return "C�digo de erro desconhecido.";
    }
}

void gpp_set_error_callback_internal(GPP_ERRORCALLBACK hcall) {
    internalErrorCallback = hcall;
}

void gpp_set_user_error_callback(GPP_ERRORCALLBACK hcall) {
    userErrorCallback = hcall;
}

void gpp_call_user_error_callback(const string& msg) {
    if (userErrorCallback) {
        userErrorCallback(msg);
    }
    else {
        cerr << "[ERRO]: " << msg << endl;
    }
}

void gpp_call_internal_error_callback(const string& msg) {
    if (internalErrorCallback) {
        internalErrorCallback(msg);
    }

    // Evita duplica��o de logs se os callbacks forem o mesmo
    if (userErrorCallback && internalErrorCallback.target<void*>() != userErrorCallback.target<void*>()) {
        userErrorCallback(msg);
    }
}
}
