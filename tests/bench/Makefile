# CFLAGS=-std=c11 -W -Wall -Wextra -Wpedantic -Werror -g -O3 -march=native -mtune=native -DSHA3_BACKEND=0
CFLAGS=-std=c11 -W -Wall -Wextra -Wpedantic -Werror -g -O3 -march=native -mtune=native
APP=./bench
OBJS=sha3.o bench.o

.PHONY=all test clean

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $(APP) $(CFLAGS) $(OBJS) -lcpucycles -lm

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) -f $(APP) $(OBJS)
