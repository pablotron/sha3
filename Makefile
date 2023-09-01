CFLAGS=-W -Wall -Wextra -Werror -pedantic -std=c11 -O3 -march=native -mtune=native
TEST_APP=./test-sha3
EXAMPLE_APP=./sha3
OBJS=sha3.o main.o

.PHONY=test all

all: $(EXAMPLE_APP)

$(EXAMPLE_APP): $(OBJS)
	$(CC) -o $(EXAMPLE_APP) $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

test:
	$(CC) -o $(TEST_APP) -DSHA3_TEST sha3.c && $(TEST_APP)

clean:
	$(RM) -f $(TEST_APP) $(EXAMPLE_APP) $(OBJS)
