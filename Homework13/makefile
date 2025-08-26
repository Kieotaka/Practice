
SERVER_SRC = server.c
CLIENT_SRC = chat_client.c
SYS_SERVER_SRC = System_server.c
SYS_CLIENT_SRC = System_client.c
POSIX_SERVER_SRC = Posix_server.c
POSIX_CLIENT_SRC = Posix_client.c


SERVER_BIN = server
CLIENT_BIN = chat_client
SYS_SERVER_BIN = System_server
SYS_CLIENT_BIN = System_client
POSIX_SERVER_BIN = Posix_server
POSIX_CLIENT_BIN = Posix_client


CC = gcc
CFLAGS = -Wall -Wextra -pedantic


LIBS = -lncurses -lpthread

all: $(SERVER_BIN) $(CLIENT_BIN) $(SYS_SERVER_BIN) $(SYS_CLIENT_BIN) $(POSIX_SERVER_BIN) $(POSIX_CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $<

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(SYS_SERVER_BIN): $(SYS_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $<

$(SYS_CLIENT_BIN): $(SYS_CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(POSIX_SERVER_BIN): $(POSIX_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $<

$(POSIX_CLIENT_BIN): $(POSIX_CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) \
	      $(SYS_SERVER_BIN) $(SYS_CLIENT_BIN) \
	      $(POSIX_SERVER_BIN) $(POSIX_CLIENT_BIN)

.PHONY: all clean