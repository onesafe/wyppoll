all :server client
CC=gcc
server:wserver.c io.c wlog.c
	$(CC) wserver.c io.c wlog.c -o server 
client:wclient.c io.c wlog.c
	$(CC) wclient.c io.c wlog.c -o client
