#include <iostream>
#include <curl/curl.h>
#include <cstdlib>

void pushToNode(const std::string& signedTxHex) {
    const char* apiKey = std::getenv("ALCHEMY_API_KEY");
    if (!apiKey) {
        std::cerr << "Error: ALCHEMY_API_KEY environment variable not set." << std::endl;
        return;
    }
    
    CURL *curl = curl_easy_init();
    if(curl) {
        std::string url = std::string("https://eth-mainnet.g.alchemy.com/v2/") + apiKey;
        std::string json = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendRawTransaction\",\"params\":[\"" + signedTxHex + "\"],\"id\":1}";
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}
