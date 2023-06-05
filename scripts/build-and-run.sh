#!/bin/bash

[ ! -d "./build" ] && mkdir -p build && cmake -S . -B ./build;

cmake --build ./build;
./build/lyli-backend;
