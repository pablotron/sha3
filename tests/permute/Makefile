CFLAGS=-W -Wall -Wextra -Werror -pedantic -std=c11 -O3 -march=native -mtune=native
APP=./test-permute
OBJS=permute.o

.PHONY=test all

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $(APP) $(CFLAGS) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

test: $(APP)
	$(APP)

clean:
	$(RM) -f $(APP) $(OBJS)
