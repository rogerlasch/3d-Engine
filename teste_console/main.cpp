
#define GPP_DOUBLE_PRECISION
#include<curl/curl.h>
#include<nlohmann/json.hpp>
using json=nlohmann::json;
#include <iostream>
#include <gpp/gpp.h>
#include"g_start.h"

using namespace gpp;
using namespace std;

const string json1="{\"context\": {\"client\": {\"hl\": \"en\", \"gl\": \"US\", \"geo\": \"US\", \"isInternal\": true, \"deviceMake\": \"\", \"deviceModel\": \"\", \"userAgent\": \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36,gzip(gfe)\", \"clientName\": \"WEB\", \"clientVersion\": \"2.20201117.05.00\", \"osName\": \"X11\", \"osVersion\": \"\", \"originalUrl\": \"https://www.youtube.com/\", \"clientFormFactor\": \"UNKNOWN_FORM_FACTOR\", \"newVisitorCookie\": true, \"countryLocationInfo\": {\"countryCode\": \"US\", \"countrySource\": \"COUNTRY_SOURCE_IPGEO_INDEX\"}, \"browserName\": \"Chrome\", \"browserVersion\": \"85.0.4183.102\", \"screenWidthPoints\": 675, \"screenHeightPoints\": 656, \"screenPixelDensity\": 1, \"screenDensityFloat\": 1, \"utcOffsetMinutes\": 60, \"userInterfaceTheme\": \"USER_INTERFACE_THEME_LIGHT\", \"connectionType\": \"CONN_CELLULAR_3G\", \"timeZone\": \"America/New_York\"}, \"user\": {\"lockedSafetyMode\": false}}, \"query\": \"$1\"}";
const         string url = "https://www.youtube.com/youtubei/v1/search?key=AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8";
const vector<string> headers={"content-type: application/json"};


// Função auxiliar para capturar a resposta do servidor
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {     size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Função para realizar uma requisição HTTP POST
bool http_post(const string& url, const string& postfields, const vector<string>& headers, string& response) {

curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Erro ao inicializar o CURL" << endl;
        return false;
    }

    // Configuração dos cabeçalhos
    struct curl_slist* headerList = NULL;
    for (const string& header : headers) {
        headerList = curl_slist_append(headerList, header.c_str());
    }

    // Configuração da requisição
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // Timeout de 10 segundos

    // Executa a requisição
    CURLcode res = curl_easy_perform(curl);

    // Verifica se houve erro
    bool success = (res == CURLE_OK);
    if (!success) {
        cerr << "Erro no HTTP POST: " << curl_easy_strerror(res) << endl;
    }

    // Limpeza
    curl_slist_free_all(headerList);
    curl_easy_cleanup(curl);
curl_global_cleanup();
    return success;
}

int main() {
try{
G_STARTER gst;

json j=json::parse(json1);
j["query"]="david herick";

string response="";

HttpService sc;
sc.start();

auto href=sc.createRequest("POST", url, j.dump(), headers);
href->waitForCompletion();

response=href->getResponse();

sc.stop();

logger::info("Response=\n{}", response);

} catch(const exception& e){
logger::info("Exception:\n{}", e.what());
}
    return 0;
}
