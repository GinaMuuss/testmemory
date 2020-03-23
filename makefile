CC=gcc
DEPS=
CFLAGS=

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
testmemory: testmemory.o
	$(CC) -o testmemory testmemory.o

format: testmemory.c
	clang-format -i testmemory.c
