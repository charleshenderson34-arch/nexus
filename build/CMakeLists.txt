cmake_minimum_required(VERSION 3.10)
project(NexusIngestion)

set(CMAKE_CXX_STANDARD 17)

# Point to the actual location where the CI/CD pipeline builds the library
set(CORE_DIR "${CMAKE_SOURCE_DIR}/external/wallet-core/build")
find_library(TRUST_WALLET_CORE NAMES TrustWalletCore PATHS ${CORE_DIR})

include_directories("${CMAKE_SOURCE_DIR}/external/wallet-core/include")

add_executable(ingest ingestion.cpp broadcaster.cpp)

# Link the library and curl
target_link_libraries(ingest ${TRUST_WALLET_CORE} curl)
