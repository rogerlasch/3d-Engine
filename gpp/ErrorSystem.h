
#ifndef GPP_ERRORSYSTEM_H
#define GPP_ERRORSYSTEM_H

#include <string>
#include <functional>

namespace gpp{

// Defini��o dos c�digos de erro
enum GPP_ERROR_CODES {
    GERROR_OK = 0,
    GERROR_UNKNOWN,
    GERROR_INVALID_PARAM,
    GERROR_NOTFOUND,
    GERROR_OPEN,
    GERROR_INVALID_STATE,
    GERROR_CONTENT,
    GERROR_FORMAT,
    GERROR_LARGE,
    GERROR_INVALID_OPERATION,
    GERROR_ALREADY,
    GERROR_EXCEPTION
};

// Callback de erro
typedef std::function<void(const std::string&)> GPP_ERRORCALLBACK;


// Recupera��o do texto do erro
std::string gpp_get_error_text(uint32_t hcode);

// Configura��o dos callbacks
void gpp_set_error_callback_internal(GPP_ERRORCALLBACK hcall);
void gpp_set_user_error_callback(GPP_ERRORCALLBACK hcall);

// Chamadas dos callbacks
void gpp_call_user_error_callback(const std::string& msg);
void gpp_call_internal_error_callback(const std::string& msg);

// Lan�ar um erro formatado (para o usu�rio e log interno)
template<class ...Args>
inline void gpp_throw_error(uint32_t code, const std::string& msg, const Args&... args) {
    std::string errorText = gpp_get_error_text(code);
    std::string formattedMsg = safe_format(msg, args...);
    std::string fullMessage = safe_format("C�digo de erro: {}, Mensagem: {}\nDetalhes: \"{}\"", code, errorText, formattedMsg);
    gpp_call_user_error_callback(fullMessage); // Envia para o usu�rio e log interno
}

// Lan�ar erro **somente interno**, sem afetar o log do usu�rio
template<class ...Args>
inline void gpp_throw_error_internal(uint32_t code, const std::string& msg, const Args&... args) {
    std::string errorText = gpp_get_error_text(code);
    std::string formattedMsg = safe_format(msg, args...);
    std::string fullMessage = safe_format("C�digo de erro interno: {}, Mensagem: {}\nDetalhes: \"{}\"", code, errorText, formattedMsg);
    gpp_call_internal_error_callback(fullMessage); // Apenas para erros internos
}
}
#endif
