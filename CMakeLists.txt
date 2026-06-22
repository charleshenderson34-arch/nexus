cmake_minimum_required(VERSION 3.10)
project(NexusIngestion)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find the Trust Wallet Core library
find_library(TRUST_WALLET_CORE libTrustWalletCore.so PATHS "${CMAKE_SOURCE_DIR}/build")

# Include directories for headers
include_directories("${CMAKE_SOURCE_DIR}/external/wallet-core/include")

# Add the ingestion engine
add_executable(ingest ingestion.cpp broadcaster.cpp)

# Link everything
target_link_libraries(ingest ${TRUST_WALLET_CORE} curl)
