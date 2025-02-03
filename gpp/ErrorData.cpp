
#include"types.h"
#include"safe_format.h"
#include"ErrorData.h"

using namespace std;

namespace gpp{
    ErrorData::ErrorData(int32 code , const string& msg ){
this->code=code;
this->msg=msg;
}

ErrorData::    ErrorData(const ErrorData& df){
*this=df;
}

    ErrorData& ErrorData::operator=(const ErrorData& df){
if(this!=&df){
this->code=df.code;
this->msg=df.msg;
}
return *this;
}

    string ErrorData::toString()const{
return safe_format("ErrorCode={}\nMsg=\"{}\"", code, msg);
}

    void ErrorData::cleanup(int32 code , const string& msg ){
this->code=code;
this->msg=msg;
}
}
