CFLAGS=-std=c11 -W -Wall -Wextra -Wpedantic -Werror -g -O3 -march=native -mtune=native
APP=./avx2
OBJS=main.o

.PHONY=all test clean

test: all
	$(APP)

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $(APP) $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) -f $(APP) $(OBJS)
