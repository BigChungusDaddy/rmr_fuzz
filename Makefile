RMR_INC_PATH := /usr/local/include/rmr/
CC := afl-clang-fast
CFLAGS := -lrmr_si -I$(RMR_INC_PATH) 

afl: harness
    RMR_SEED_RT=./routes.rt afl-fuzz -i in -o out ./harness abcd

harness: harness.c
    $(CC) -o harness harness.c $(CFLAGS)