cmake_minimum_required(VERSION 3.10)

project(lyli-backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

include_directories(src)

OPTION(LOG_TRACE "Toggle logger output" ON)
OPTION(LOG_DEBUG "Toggle debug output" OFF)

IF(LOG_TRACE)
    ADD_DEFINITIONS(-DLOG_TRACE=1)
ELSE()
    ADD_DEFINITIONS(-DLOG_TRACE=0)
ENDIF(LOG_TRACE)

IF(LOG_DEBUG)
    ADD_DEFINITIONS(-DLOG_DEBUG=1)
ELSE()
    ADD_DEFINITIONS(-DLOG_DEBUG=0)
ENDIF(LOG_DEBUG)

add_executable(lyli-backend
    src/Server/TcpServer.cpp
    src/Server/TcpConnection.cpp
    src/Utils/Logger.cpp
    src/main.cpp
)

target_link_libraries(lyli-backend PUBLIC nlohmann_json)