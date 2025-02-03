

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

bool isOpen()const;
void open(const std::string& filename, const std::string& key, ErrorData* error=NULL);
void close();
void pushFile(const std::string& filename, const std::string& internal_name, ErrorData* error=NULL);
void extractFile(const std::string& internal_name, const std::string& filename, ErrorData* error=NULL);
void removeFile(const std::string& filename, ErrorData* error=NULL);
bool exists(const std::string& filename)const;
FileDataRef getFile(const std::string& filename, ErrorData* error=NULL);
void listFiles(std::vector<std::pair<std::string, int64>>& hlist);
void update();

private:

bool existsInDb(const std::string& filename)const;
FileDataRef getFileDb(const std::string& filename, ErrorData* error=NULL);
};
}
#endif
