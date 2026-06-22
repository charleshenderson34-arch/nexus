#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TrustWalletCore/AnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/Ethereum/Proto/Ethereum.pb.h>

void pushToNode(const std::string& signedTxHex);

std::vector<std::string> parseCSVRow(const std::string& rowText, char delimiter) {
    std::vector<std::string> columns;
    std::string column;
    std::istringstream stream(rowText);
    while (std::getline(stream, column, delimiter)) {
        columns.push_back(column);
    }
    return columns;
}

void reconstructAndSign(const std::string& privKey, const std::string& destAddress, const std::string& weiAmount, const std::string& txNonce, const std::string& gasPriceVal) {
    TW::Ethereum::Proto::SigningInput input;
    
    input.set_to_address(destAddress);
    input.set_chain_id("1"); 
    
    uint64_t finalNonce = std::stoull(txNonce);
    input.set_nonce(finalNonce);
    
    uint64_t finalGasPrice = std::stoull(gasPriceVal);
    input.set_gas_price(finalGasPrice);
    input.set_gas_limit(21000); 
    
    input.set_amount(weiAmount);
    input.set_private_key(privKey);

    std::string serializedInput = input.SerializeAsString();
    auto serializedOutput = TW::AnySigner::sign(serializedInput, TWCoinTypeEthereum);
    
    TW::Ethereum::Proto::SigningOutput output;
    output.ParseFromString(serializedOutput);

    if (!output.encoded().empty()) {
        std::cout << "Reconstruction complete for target destination: " << destAddress << std::endl;
        pushToNode(output.encoded());
    } else {
        std::cerr << "Cryptographic signature generation failed for target row." << std::endl;
    }
}

int main() {
    std::ifstream recordFile("agave-3.1.14.csv");
    if (!recordFile.is_open()) {
        std::cerr << "Fatal Error: Missing required portfolio record input file: agave-3.1.14.csv" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(recordFile, line)) {
        if (line.empty()) continue;
        std::vector<std::string> data = parseCSVRow(line, ',');
        if (data.size() < 5) {
            std::cerr << "Skipping invalid row layout: " << line << std::endl;
            continue;
        }
        
        reconstructAndSign(data[0], data[1], data[2], data[3], data[4]);
    }
    return 0;
}
