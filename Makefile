CC=gcc
CFLAGS=-I.
DEPS = flexclog.h
OBJ = flexclog.o
LIB = libfclog.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJ)
	ar rcs $@ $^

.PHONY: install
install:
	mkdir -p /usr/local/include
	mkdir -p /usr/local/lib
	cp flexclog.h /usr/local/include/
	cp $(LIB) /usr/local/lib/

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB)