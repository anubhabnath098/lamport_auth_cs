cmake_minimum_required(VERSION 3.16)
project(LamportAuthProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Find Crypto++ manually
find_path(CRYPTOPP_INCLUDE_DIR cryptopp/cryptlib.h
    PATHS C:/Users/anubh/vcpkg/installed/x64-mingw-static/include
)

find_library(CRYPTOPP_LIBRARY
    NAMES cryptopp cryptopp-static cryptlib
    PATHS C:/Users/anubh/vcpkg/installed/x64-mingw-static/lib
)

if(NOT CRYPTOPP_INCLUDE_DIR OR NOT CRYPTOPP_LIBRARY)
    message(FATAL_ERROR "Crypto++ not found. Install using vcpkg with MinGW toolchain.")
endif()

message(STATUS "Found Crypto++ include: ${CRYPTOPP_INCLUDE_DIR}")
message(STATUS "Found Crypto++ library: ${CRYPTOPP_LIBRARY}")

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/hashchain.cpp
    src/networkmanager.cpp
    src/protocolhandler.cpp
)

set(HEADERS
    src/mainwindow.h
    src/hashchain.h
    src/networkmanager.h
    src/protocolhandler.h
)

# Use absolute path for UI file
set(UI_FILES
    ${CMAKE_SOURCE_DIR}/src/mainwindow.ui
)

qt6_add_executable(LamportAuthProtocol ${SOURCES} ${HEADERS} ${UI_FILES})

target_include_directories(LamportAuthProtocol PRIVATE ${CRYPTOPP_INCLUDE_DIR})

target_link_libraries(LamportAuthProtocol
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Network
    PRIVATE ${CRYPTOPP_LIBRARY}
)

if(WIN32)
    target_link_libraries(LamportAuthProtocol PRIVATE ws2_32)
endif()

configure_file(${CMAKE_SOURCE_DIR}/config.ini ${CMAKE_BINARY_DIR}/config.ini COPYONLY)

add_custom_target(run
    COMMAND LamportAuthProtocol
    DEPENDS LamportAuthProtocol
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running LamportAuthProtocol"
)
