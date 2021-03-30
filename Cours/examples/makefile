CC=gcc

exe = server client1 client2 client

all : $(exe)

server: dgramInit.c
	$(CC) -DSERVER $^ -o build/$@

client1: dgramInit.c
	$(CC) -DCLIENT1 $^ -o build/$@

client2: dgramInit.c
	$(CC) -DCLIENT2 $^ -o build/$@

client: dgramInit.c
	$(CC) -DCLIENT $^ -o build/$@
