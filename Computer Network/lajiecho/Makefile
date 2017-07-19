CC = gcc
CFLAGS = -g

all: echo_srv.elf echo_cli.elf
	@echo "Done\n"

echo_srv.elf:
	$(CC) lajiecho.c $(CFLAGS) -o $@ 
	
echo_cli.elf:
	$(CC) lajiecho_client.c $(CFLAGS) -o $@

clean:
	rm ./*.elf

.PHONY: clean all
