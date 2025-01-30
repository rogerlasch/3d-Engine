

#ifndef GPP_CURLDATA_H
#define GPP_CURLDATA_H

namespace gpp{

class CurlData{
public:
CURL* handle;
struct curl_slist* headers;

CurlData(){
handle=NULL;
headers=NULL;
}
};
};
#endif
