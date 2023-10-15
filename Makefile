# compiler flags used for sample application and shared library
CFLAGS=-W -Wall -Wextra -Werror -pedantic -std=c11 -fPIC -O3 -march=native -mtune=native

# sample application
APP=./sha3
APP_OBJS=sha3.o main.o

# shared library
LIB=libsha3.so
LIB_OBJS=sha3.o

# test app (test suite and sanitizers)
TEST_CFLAGS=-g -fsanitize=address,pointer-compare,pointer-subtract,undefined,leak -W -Wall -Wextra -Werror -pedantic -std=c11
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
	$(CC) -o $(TEST_APP) $(TEST_CFLAGS) -DSHA3_TEST sha3.c && $(TEST_APP)

# build API documentation (requires doxygen)
doc:
	doxygen sha3.h

# remove build files
clean:
	$(RM) -f $(TEST_APP) $(APP) $(APP_OBJS) $(LIB) $(LIB_OBJS)
