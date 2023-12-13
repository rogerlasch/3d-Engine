

#include<fstream>
#include<string>
#include<vector>
#include"../types.h"
#include"gpp_exception.h"
#include"logger.h"

using namespace std;

namespace gpp
{

logger::logger(const string& filename, int level)
{
ofn.open(filename);
if(!ofn.is_open())
{
_GEXCEPT("Erro ao abrir o arquivo: "+filename);
}
this->filename=filename;
this->level=level;
this->flags=0;
stacktrace.clear();
}

logger::~logger()
{
ofn.close();
this->filename=filename;
this->level=level;
stacktrace.clear();
}
}
