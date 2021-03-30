CC=gcc

exe = server client

all : $(exe)

server: dgramInit.c
	$(CC) -DSERVER $^ -o $@

client: dgramInit.c
	$(CC) -DCLIENT $^ -o $@
