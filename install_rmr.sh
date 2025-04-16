#!/usr/bin/env bash
cd ..
git clone https://github.com/o-ran-sc/ric-plt-lib-rmr
cd ./ric-plt-lib-rmr
mkdir build
cd build
export AFL_LLVM_LAF_ALL=1
export AFL_LLVM_NGRAM_SIZE=4
export AFL_LLVM_CALLER=1
cmake -D CMAKE_C_COMPILER=afl-clang-fast CMAKE_CXX_COMPILER=afl-clang-fast++ -DDEV_PKG=1 ..
make rmr_si_static
cp librmr_si.a /users/axetang/rmr_fuzz/librmr_si.a
cd ..
cp ./src/rmr/common/include/rmr.h /users/axetang/rmr_fuzz/rmr.h
cd ..
export RMR_SEED_RT=./routes.rt
export RMR_RTG_SVC=-1