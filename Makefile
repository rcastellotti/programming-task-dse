FLAGS=  -Wall -Wextra -pedantic -Werror --ansi -DLOG_USE_COLOR -lpthread --std=gnu11 
DEPS= map.c log.c list.c

all: client server 
client: client.c $(DEPS)
		$(CC) client.c $(DEPS) $(FLAGS) -o client 
server: server.c $(DEPS)
		$(CC) server.c $(DEPS) $(FLAGS) -o server 

debug: client_debug server_debug
client_debug: client.c $(DEPS)
		$(CC) client.c $(DEPS) -g  -DDEBUG $(FLAGS) -o client_debug
server_debug: server.c $(DEPS)
		$(CC) server.c $(DEPS) -g -DDEBUG $(FLAGS) -o server_debug
clean:
	rm -f server && rm -f client && rm -f server_debug && rm -f client_debug && rm -rf *.dSYM