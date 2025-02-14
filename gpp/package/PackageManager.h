

#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include<string>
#include<unordered_map>
#include<atomic>
#include<sqlcipher/sqlite3.h>
#include"FileData.h"

namespace gpp{

class PackageManager{
private:
std::string filename;
std::string key;
std::unordered_map<std::string, std::shared_ptr<FileData>> hfiles;

int64 cacheDuration;
int64 cacheFrequence;
int64 startFrequenceTime;
uint32 errorCode;
sqlite3* db;

public:
PackageManager();
PackageManager(const PackageManager& pg)=delete;
PackageManager& operator=(const PackageManager& pg)=delete;
~PackageManager();

void setCacheDuration(int64 hseconds){
this->cacheDuration=hseconds;
if(cacheDuration<0)cacheDuration=0;
}
int64 getCacheDuration()const{return cacheDuration;}

inline uint32 getErrorCode()const{return errorCode;}
bool isOpen()const;
void open(const std::string& filename, const std::string& key);
void close();
void pushFile(const std::string& filename, const std::string& internal_name);
void extractFile(const std::string& internal_name, const std::string& filename);
void removeFile(const std::string& filename);
bool exists(const std::string& filename)const;
FileDataRef getFile(const std::string& filename);
void listFiles(std::vector<std::pair<std::string, int64>>& hlist);
void update();

private:

bool existsInDb(const std::string& filename)const;
FileDataRef getFileDb(const std::string& filename);

template<class ...Args>
void handleError(uint32 code, const std::string& msg, const Args&... args) {
    this->errorCode = code;

    if (this->errorCode != GERROR_OK) {
        gpp_throw_error_internal(code, msg, args...);
    }
}
};
}
#endif
