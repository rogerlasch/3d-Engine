
#ifndef ERRORDATA_H
#define ERRORDATA_H

#include <string>
#include <utility>

namespace gpp{
class ErrorData {
public:
    int32 code;
    std::string msg;

    // Construtor padr�o e com par�metros
    ErrorData(int32 code = 0, const std::string& msg = "");
    ErrorData(const ErrorData& df);
    ErrorData& operator=(const ErrorData& df);
    ~ErrorData() = default;

    std::string toString() const;
    void cleanup(int32 code = 0, const std::string& msg = "");

    void setCode(int32 code) { this->code = code; }
    int32 getCode() const { return code; }

    void setMsg(const std::string& msg) { this->msg = msg; }
    std::string getMsg() const { return msg; }

    // Define o c�digo e a mensagem com formata��o opcional
    template <typename... Args>
inline     void define(int32 code, const std::string& msg, const Args&... args) {
        this->code = code;
        this->msg = safe_format(msg, args...);
    }
};
}
#endif // ERRORDATA_H
