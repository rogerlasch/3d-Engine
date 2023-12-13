

#ifndef CURLHANDLER_H
#define CURLHANDLER_H

struct CurlHandler{
CURL* handle=NULL;
struct curl_slist* headers=NULL;
};

#endif
