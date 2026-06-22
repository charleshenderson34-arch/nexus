#include <iostream>
#include <TrustWalletCore/AnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/Ethereum/Proto/Ethereum.pb.h>

void reconstructAndSign(const std::string& privateKeyHex, const std::string& toAddress, const std::string& amount) {
    // Construct the Ethereum Signing Input
    Ethereum::Proto::SigningInput input;
    input.set_to_address(toAddress);
    input.set_chain_id("1"); // Chain ID 1 for Ethereum Mainnet
    input.set_nonce(0);      // Note: Must fetch current nonce from node
    input.set_gas_price(20000000000); 
    input.set_gas_limit(21000);
    input.set_amount(amount);
    input.set_private_key(privateKeyHex);

    // Sign the transaction
    auto inputData = input.SerializeAsString();
    auto outputData = AnySigner::sign(inputData, TWCoinTypeEthereum);
    
    Ethereum::Proto::SigningOutput output;
    output.ParseFromString(outputData);

    std::cout << "Reconstructed Transaction Hex: " << output.encoded() << std::endl;
}

int main() {
    std::cout << "Ingestion Engine Initialized." << std::endl;
    // CSV parsing loop goes here
    return 0;
}
