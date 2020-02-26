CC=gcc
CFLAGS=-I.
DEPS=main.h
ODIR=out

_OBJ=main.o
OBJ= $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compile: setup main

setup:
	@mkdir -p $(ODIR)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(ODIR) main

.PHONY: clean compile setup
