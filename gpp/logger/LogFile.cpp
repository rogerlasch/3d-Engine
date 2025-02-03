
#include<iostream>
#include<fstream>
#include"../types.h"
#include"LogItem.h"
#include"LogFile.h"

using namespace std;

namespace gpp{
LogFile::LogFile(const string& filename, bool print){
this->filename=((filename.size()==0) ? "Default log file.txt" : filename);
this->print=print;
}

 void LogFile::init(){
ofn.open(filename);
}

 void LogFile::end(){
if(ofn.is_open()){
ofn.close();
}
}

 void LogFile::dumpMessage(uint32 level, int64_t timestamp, const std::string& msg){
ofn<<msg<<endl;
if(print){
cout<<msg<<endl;
}
}
}
