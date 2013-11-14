all :server client
CC=gcc
server:wserver.c io.c
	$(CC) wserver.c io.c -o server 
client:wclient.c io.c
	$(CC) wclient.c io.c -o client
