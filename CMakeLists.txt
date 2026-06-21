cmake_minimum_required(VERSION 3.15)
project(NexusWalletCore)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-m32)

# Scan for sub-projects
# This tells make to enter the external folders
add_subdirectory(external/wallet-core)
add_subdirectory(external/BigInt)

message(STATUS "Nexus Build Configuration Loaded with Targets")
