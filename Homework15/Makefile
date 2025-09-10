CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: receiver1 sender1 block_sigint sigwait_loop

receiver1: receiver1.c
	$(CC) $(CFLAGS) -o receiver1 receiver1.c

sender1: sender1.c
	$(CC) $(CFLAGS) -o sender1 sender1.c

block_sigint: block_sigint.c
	$(CC) $(CFLAGS) -o block_sigint block_sigint.c

sigwait_loop: sigwait_loop.c
	$(CC) $(CFLAGS) -o sigwait_loop sigwait_loop.c

clean:
	rm -f receiver1 sender1 block_sigint sigwait_loop

.PHONY: all clean