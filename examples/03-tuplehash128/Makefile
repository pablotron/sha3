CFLAGS=-W -Wall -Wextra -Werror -pedantic -std=c11 -O3 -march=native -mtune=native
APP=./tuplehash128-example
OBJS=sha3.o main.o

.PHONY=all

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $(APP) $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) -f $(APP) $(OBJS)
