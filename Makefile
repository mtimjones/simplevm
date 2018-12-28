CC=gcc
CFLAGS = -I. -g

DEPS = simplevm.h
OBJS = machine.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

vm: $(OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS)

clean:
	rm -f vm *.o
