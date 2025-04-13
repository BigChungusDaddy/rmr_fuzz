#!/usr/bin/env bash
cd ..
git clone https://github.com/o-ran-sc/ric-plt-lib-rmr
cd ./ric-plt-lib-rmr
mkdir build
cd build
cmake -D CMAKE_C_COMPILER=afl-clang-fast CMAKE_CXX_COMPILER=afl-clang-fast++ -DDEV_PKG=1 ..
cd ..
cd ..