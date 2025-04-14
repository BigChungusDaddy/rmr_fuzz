RMR_INC_PATH := /usr/local/include/rmr/
CC := afl-clang-fast
CFLAGS := -lrmr_si -I$(RMR_INC_PATH) 

afl: harness
    RMR_SEED_RT=./routes.rt RMR_RTG_SVC=4560 afl-fuzz -i in -o out ./harness @@

harness: harness.c
    afl-clang-fast -o harness harness.c librmr_si.a