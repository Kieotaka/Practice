
CC = gcc

CFLAGS = -Wall -Wextra -std=c11

LAB121 = lab121
SERVER = server
CLIENT = client
MYSHELL = myshell

SRCS_LAB121 =lab121.c
SRCS_SERVER =lab122_serv.c
SRCS_CLIENT =lab122_client.c
SRCS_MYSHELL =lab123.c

OBJS_LAB121 = $(SRCS_LAB121:.c=.o)
OBJS_SERVER = $(SRCS_SERVER:.c=.o)
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)
OBJS_MYSHELL = $(SRCS_MYSHELL:.c=.o)


.PHONY: all clean

all: $(SERVER) $(CLIENT) $(MYSHELL) $(LAB121)

$(LAB121): $(OBJS_LAB121)
	$(CC) -o $@ $^

$(SERVER): $(OBJS_SERVER)
	$(CC) -o $@ $^

$(CLIENT): $(OBJS_CLIENT)
	$(CC) -o $@ $^

$(MYSHELL): $(OBJS_MYSHELL)
	$(CC) -o $@ $^
	

clean:
	rm -f $(SERVER) $(CLIENT) $(MYSHELL) $(LAB121) *.o