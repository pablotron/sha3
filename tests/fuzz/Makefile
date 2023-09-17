CFLAGS=-g -fsanitize=address,fuzzer -W -Wall -Wextra -Werror -pedantic -std=c11
APP=./fuzz-shake128
OBJS=sha3.o fuzz.o
CC=clang

.PHONY=all clean fuzz

all: $(APP)

# build sample application
$(APP): $(OBJS)
	$(CC) -o $(APP) $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

fuzz: $(APP)
	$(APP)

clean:
	$(RM) -f $(APP) $(OBJS)
