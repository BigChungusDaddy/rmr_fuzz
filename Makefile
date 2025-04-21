RMR_INC_PATH := /usr/local/include/rmr/
CC := afl-clang-fast
CFLAGS := -lrmr_si -I$(RMR_INC_PATH) 

afl: harness
    RMR_SEED_RT=./routes.rt RMR_RTG_SVC=-1 afl-fuzz -i in -o out ./harness_old @@

harness: harness_old.c
    gcc -g -o harness_d harness_old.c librmr_si_d.a