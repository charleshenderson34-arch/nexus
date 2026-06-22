#include <iostream>
#include <curl/curl.h>
#include <cstdlib>
#include <string>

void pushToNode(const std::string& signedTxHex) {
    const char* apiKey = std::getenv("ALCHEMY_API_KEY");
    const char* rpcUrlEnv = std::getenv("RPC_URL");
    
    if (!apiKey && !rpcUrlEnv) {
        std::cerr << "Error: Neither ALCHEMY_API_KEY nor RPC_URL environment variables are configured." << std::endl;
        return;
    }
    
    std::string url;
    if (rpcUrlEnv) {
        url = std::string(rpcUrlEnv);
    } else {
        url = std::string("https://eth-mainnet.g.alchemy.com/v2/") + apiKey;
    }
    
    CURL *curl = curl_easy_init();
    if (curl) {
        std::string jsonPayload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendRawTransaction\",\"params\":[\"0x" + signedTxHex + "\"],\"id\":1}";
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL Broadcast Failure: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Transaction payload successfully transmitted to network node." << std::endl;
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}
