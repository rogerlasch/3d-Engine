
#include <fstream>
#include <sstream>
#include"../debug_system.h"
#include "PackageManager.h"

using namespace std;

namespace gpp{
PackageManager::PackageManager() : db(nullptr), cacheDuration(300), cacheFrequence(60) {
startFrequenceTime=get_timestamp_sec();
}

PackageManager::~PackageManager() {
    if (isOpen()) {
        close();
    }
}

bool PackageManager::isOpen() const {
    return db != nullptr;
}

void PackageManager::open(const string& filename, const string& key) {

handleError(GERROR_OK, "");

    if (isOpen()) {
handleError(GERROR_ALREADY, "O pacote de arquivos j� est� aberto. N�o � poss�vel abrir outro antes de fechar novamente.");
        return;
    }

    int32 res = sqlite3_open(filename.c_str(), &db);
    if (res != SQLITE_OK) {
handleError(GERROR_OPEN, "O banco de dados n�o pode ser aberto.\n{}", sqlite3_errmsg(db));
        return;
    }

    // Configura a chave de criptografia
    string pragma = "PRAGMA key = '" + key + "';";
    res = sqlite3_exec(db, pragma.c_str(), nullptr, nullptr, nullptr);
    if (res != SQLITE_OK) {
handleError(GERROR_INVALID_OPERATION, "Falha ao definir a chave de descriptografia do banco de dados.\n{}", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = nullptr;
        return;
    }

    // Cria a tabela se n�o existir
    string sql = R"(
        CREATE TABLE IF NOT EXISTS files (
            filename TEXT PRIMARY KEY,
            data BLOB NOT NULL,
            size INTEGER NOT NULL
        );
    )";
    res = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    if (res != SQLITE_OK) {
handleError(GERROR_INVALID_OPERATION, "Erro ao criar a tabela para armazenar os arquivos do pacote.\n{}", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = nullptr;
        return;
    }

    this->filename = filename;
    this->key = key;
startFrequenceTime=get_timestamp_sec();
}

void PackageManager::close() {
handleError(GERROR_OK, "");
    if (isOpen()) {
        sqlite3_close(db);
        db = nullptr;
        hfiles.clear();
    }
}

void PackageManager::pushFile(const string& filePath, const string& internal_name) {

handleError(GERROR_OK, "");

    if (!isOpen()) {
handleError(GERROR_INVALID_STATE, "O banco de dados do pacote n�o est� aberto para realizar esta opera��o.");
        return;
    }

if(internal_name==""){
handleError(GERROR_INVALID_PARAM, "O nome interno do arquivo � inv�lido ou n�o foi definido.");
return;
}

    ifstream ifs(filePath, ios::binary);
    if (!ifs.is_open()) {
handleError(GERROR_OPEN, "O arquivo {} n�o pode ser aberto.", filePath);
        return;
    }

    // L� o arquivo para um buffer
    ifs.seekg(0, ios::end);
    size_t fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    vector<int8> buffer(fileSize);
    ifs.read((char*)buffer.data(), fileSize);
ifs.close();

    // Insere o arquivo no banco
    const char* sql = "INSERT INTO files (filename, data, size) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (res != SQLITE_OK) {
handleError(GERROR_INVALID_OPERATION, "Erro ao preparar o arquivo {} para o pacote. Erro:\n{}", filePath, sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, internal_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, buffer.data(), fileSize, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, fileSize);

    res = sqlite3_step(stmt);
    if (res != SQLITE_DONE) {
handleError(GERROR_INVALID_OPERATION, "Erro ao adicionar o arquivo {} para o pacote. Erro:\n{}", filePath, sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void PackageManager::extractFile(const string& internal_name, const string& filename){

handleError(GERROR_OK, "");

auto hfile=getFile(internal_name);

if(hfile==NULL){
handleError(GERROR_NOTFOUND, "Erro ao buscar o arquivo interno {}. Ele n�o existe!", internal_name);
return;
}

ofstream ofn(filename, ios::binary);

if(!ofn.is_open()){
handleError(GERROR_OPEN, "Erro ao abrir o arquivo {} para grava��o.", filename);
return;
}

ofn.write((char*)hfile->getData(), hfile->getSize());
ofn.close();

}

void PackageManager::removeFile(const string& filename) {

handleError(GERROR_OK, "");

    if (!isOpen()) {
handleError(GERROR_INVALID_STATE, "O banco de dados do pacote n�o est� aberto para realizar esta opera��o.");
        return;
    }

    const char* sql = "DELETE FROM files WHERE filename = ?;";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (res != SQLITE_OK) {
handleError(GERROR_INVALID_OPERATION, "Erro ao excluir o arquivo {} do pacote.\n{}", filename, sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, filename.c_str(), -1, SQLITE_STATIC);

    res = sqlite3_step(stmt);
    if (res != SQLITE_OK) {
handleError(GERROR_INVALID_OPERATION, "Erro ao excluir o arquivo {} do pacote.\n{}", filename, sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

bool PackageManager::exists(const string& filename) const {
    return hfiles.find(filename) != hfiles.end() || existsInDb(filename);
}

FileDataRef PackageManager::getFile(const string& filename) {

handleError(GERROR_OK, "");

    auto it = hfiles.find(filename);
    if (it != hfiles.end()) {
        return it->second->getReference();
    }

return getFileDb(filename);
}

void PackageManager::listFiles(vector<pair<string, int64>>& hlist) {
    if (!isOpen()) {
handleError(GERROR_INVALID_STATE, "O banco de dados do pacote n�o est� aberto para realizar esta opera��o.");
return;
}

    const char* sql = "SELECT filename, size FROM files;";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (res != SQLITE_OK) return;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string filename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int64 size = sqlite3_column_int64(stmt, 1);
        hlist.emplace_back(filename, size);
    }

    sqlite3_finalize(stmt);
}


void PackageManager::update(){

int64 tend=get_timestamp_sec();
int64 ts=tend-startFrequenceTime;

if(ts<cacheFrequence){
return;
}

startFrequenceTime=get_timestamp_sec();

if(hfiles.empty()){
return;
}

vector<string> hnames;

for(auto& it : hfiles){
if(it.second->canRemove(cacheDuration)){
hnames.push_back(it.first);
}
}

for(auto& name : hnames){
auto it=hfiles.find(name);
if(it!=hfiles.end()){
it->second->cleanup();
hfiles.erase(it);
}
}
}

bool PackageManager::existsInDb(const string& filename)const {
    if (!isOpen()) return false;

    const char* sql = "SELECT COUNT(*) FROM files WHERE filename = ?;";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (res != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, filename.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}

FileDataRef PackageManager::getFileDb(const string& filename) {
    if (!isOpen()) return nullptr;

    const char* sql = "SELECT data FROM files WHERE filename = ?;";
    sqlite3_stmt* stmt;
    int32 res = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (res != SQLITE_OK) {
handleError(GERROR_INVALID_OPERATION, "{}", sqlite3_errmsg(db));
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, filename.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
const         void* blob = sqlite3_column_blob(stmt, 0);
        int32 size = sqlite3_column_bytes(stmt, 0);
        if (blob && size > 0) {

            auto fileData = make_shared<FileData>();
fileData->filename=filename;
fileData->data.resize(size);
memcpy(fileData->data.data(), blob, size);

            hfiles[filename] = fileData;

    sqlite3_finalize(stmt);
return fileData->getReference();
        }

    sqlite3_finalize(stmt);
    return FileDataRef();
    }

    sqlite3_finalize(stmt);
    return FileDataRef();
}
}
