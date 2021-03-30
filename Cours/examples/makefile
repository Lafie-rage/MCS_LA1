CC=gcc

exe = server client1 client2 client

all : $(exe)

server: dgramInit.c
	$(CC) -DSERVER $^ -o $@

client1: dgramInit.c
	$(CC) -DCLIENT1 $^ -o $@

client2: dgramInit.c
	$(CC) -DCLIENT2 $^ -o $@

client: dgramInit.c
	$(CC) -DCLIENT $^ -o $@
