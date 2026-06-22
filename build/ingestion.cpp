#include <iostream>
#include <fstream>
#include <string>
#include <TrustWalletCore/AnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/Ethereum/Proto/Ethereum.pb.h>

void transformAndReconstruct(const std::string& symbol, const std::string& amount, const std::string& toAddress) {
    Ethereum::Proto::SigningInput input;
    input.set_to_address(toAddress);
    input.set_chain_id("1");
    input.set_nonce(0); 
    input.set_gas_price(20000000000);
    input.set_gas_limit(21000);
    input.set_amount(amount);
    
    std::cout << "Transforming Agave asset: " << symbol << " Amount: " << amount << std::endl;
}

int main() {
    std::ifstream file("agave-3.1.14.csv");
    std::string line;
    while (std::getline(file, line)) {
        transformAndReconstruct("AGAVE", "100", "0xTargetAddress");
    }
    return 0;
}
