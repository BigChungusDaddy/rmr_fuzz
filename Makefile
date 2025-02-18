RMR_INC_PATH := /usr/local/include/rmr/
CC := afl-clang-fast
CFLAGS := -lrmr_si -I$(RMR_INC_PATH)

afl: harness
  afl-fuzz -i in -o out ./harness abcd

harness: harness
$(CC) $(CFLAGS) -o harness harness.c