#!/bin/bash

[ ! -d "./build" ] && mkdir -p build && cmake -S . -B ./build -DCMAKE_CXX_COMPILER=/bin/clang++;

cmake --build ./build;
./build/lyli-backend;
