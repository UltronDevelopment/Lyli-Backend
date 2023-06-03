cmake_minimum_required(VERSION 3.10)

project(lyli-backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

include_directories(src)

option(TRACE "Toggle logger output" ON)
option(DEBUG "Toggle debug output" OFF)

if(TRACE)
    add_definitions(-DLOG_TRACE=1)
else()
    add_definitions(-DLOG_TRACE=0)
endif(TRACE)

if(DEBUG)
    add_definitions(-DLOG_DEBUG=1)
else()
    add_definitions(-DLOG_DEBUG=0)
ENDIF(DEBUG)

add_compile_options(-Wall -O2)

add_executable(lyli-backend
    src/Dotenv/Loader.cpp
    src/Dotenv/Parser.cpp
    src/Server/HTTP/HttpRequest.cpp
    src/Server/TcpServer.cpp
    src/Server/TcpConnection.cpp
    src/Utils/Logger.cpp
    src/main.cpp
)

target_link_libraries(lyli-backend PUBLIC nlohmann_json)
