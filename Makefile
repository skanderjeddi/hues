CC=gcc
CFLAGS=-I.
DEPS = hues.h
OBJ = hues.o
LIB = libhues.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJ)
	ar rcs $@ $^

.PHONY: install
install:
	mkdir -p /usr/local/include
	mkdir -p /usr/local/lib
	cp hues.h /usr/local/include/
	cp $(LIB) /usr/local/lib/

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB)