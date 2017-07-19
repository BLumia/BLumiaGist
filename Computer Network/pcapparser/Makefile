CC = gcc
CFLAGS = -g

all: pcapparser.elf
	@echo "Done\n"

pcapparser.elf:
	$(CC) main.c $(CFLAGS) -o $@ 

clean:
	rm ./*.elf

.PHONY: clean all
