CC=gcc
BUILD_FOLDER := build

all : $(addprefix $(BUILD_FOLDER)/,server_fork server_thread client)

$(BUILD_FOLDER)/server_fork: streamInit.c
	$(CC) -DSERVER_FORK $^ -o $@ -lpthread

$(BUILD_FOLDER)/server_thread: streamInit.c
	$(CC) -DSERVER_THREAD $^ -o $@ -lpthread

$(BUILD_FOLDER)/client: streamInit.c
	$(CC) -DCLIENT $^ -o $@ -lpthread

$(BUILD_FOLDER):
	mkdir -p build/

clean :
	rm build/*
