cmake_minimum_required(VERSION 3.10)

project(lyli-backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)
find_package(mongoc-1.0 1.15 REQUIRED)

include_directories(src)
include_directories(/usr/include/libmongoc-1.0)
include_directories(/usr/include/libbson-1.0)

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

add_compile_options(-Wall -Wextra -Werror -O2)

add_executable(lyli-backend
    src/API/Handler/Ping.cpp
    src/API/Handler/Register.cpp
    src/API/Router.cpp
    src/DB/Client.cpp
    src/DB/Collection.cpp
    src/DB/Database.cpp
    src/Dotenv/Loader.cpp
    src/Dotenv/Parser.cpp
    src/Server/HTTP/HttpObject.cpp
    src/Server/HTTP/HttpRequest.cpp
    src/Server/HTTP/HttpResponse.cpp
    src/Server/TcpServer.cpp
    src/Server/TcpConnection.cpp
    src/Utils/FormattedTime.cpp
    src/Utils/Logger.cpp
    src/main.cpp
)

target_link_libraries(lyli-backend PUBLIC nlohmann_json pthread PRIVATE mongo::mongoc_shared)
