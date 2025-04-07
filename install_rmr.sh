#!/usr/bin/env bash
git clone https://github.com/o-ran-sc/ric-plt-lib-rmr
cd ./ric-plt-lib-rmr
mkdir build
cd build
cmake -D CMAKE_C_COMPILER=afl-clang-fast CMAKE_CXX_COMPILER=afl-clang-fast++ ..
sudo make install
cd ..
cd ..