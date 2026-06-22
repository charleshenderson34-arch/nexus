#include <iostream>
#include <curl/curl.h>

void pushToNode(const std::string& signedTxHex) {
    CURL *curl = curl_easy_init();
    if(curl) {
        std::string json = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_sendRawTransaction\",\"params\":[\"" + signedTxHex + "\"],\"id\":1}";
        curl_easy_setopt(curl, CURLOPT_URL, "https://eth-mainnet.g.alchemy.com/v2/YOUR_API_KEY");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}
