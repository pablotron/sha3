APP=neon
CFLAGS=-std=c11 -O2 -W -Wall -Wextra -Werror -pedantic -march=native -mtune=native
OBJS=neon.o

.PHONY=all clean test

all: $(APP)

test: all
	./neon

$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $(APP) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) -f $(APP) $(OBJS)
