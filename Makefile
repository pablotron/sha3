# backend (0 to auto-detect)
BACKEND ?= 0

# compiler flags used for sample application and shared library
CFLAGS=-W -Wall -Wextra -Werror -pedantic -std=c11 -fPIC -O3 -march=native -mtune=native -DBACKEND=$(BACKEND)

# sample application
APP=./sha3
APP_OBJS=sha3.o main.o

# shared library
LIB=libsha3.so
LIB_OBJS=sha3.o

# test app (test suite and sanitizers)
#
# note: some older versions of GCC may have trouble with these sanitizer
# options.  if the compiler is giving you grief, try changing the
# "-fsanitize" parameter to "-fsanitize=undefined" or removing the
# "-fsanitize" parameter completely
#
# as noted in https://github.com/pablotron/fips203ipd/issues/1, macos
# clang and gcc do not support -fsanitize=leak, so i have removed it for
# now
TEST_CFLAGS=-g -fsanitize=address,pointer-compare,pointer-subtract,undefined -W -Wall -Wextra -Werror -pedantic -std=c11 -march=native -mtune=native -DBACKEND=$(BACKEND)
TEST_APP=./test-sha3

.PHONY=test all

all: $(APP) $(LIB)

# build sample application
$(APP): $(APP_OBJS)
	$(CC) -o $(APP) $(CFLAGS) $(APP_OBJS)

# build shared library
$(LIB): $(LIB_OBJS)
	$(CC) -shared -o $(LIB) $(CFLAGS) $(LIB_OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

# build and run test suite with sanitizers
test:
	$(CC) -o $(TEST_APP) $(TEST_CFLAGS) -DTEST_SHA3 sha3.c && $(TEST_APP)

# build API documentation (requires doxygen)
doc:
	doxygen

# remove build files
clean:
	$(RM) -f $(TEST_APP) $(APP) $(APP_OBJS) $(LIB) $(LIB_OBJS)
