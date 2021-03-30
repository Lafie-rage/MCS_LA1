CC=gcc
BUILD_FOLDER := build

all : $(addprefix $(BUILD_FOLDER)/,server client client_response_no_bind) multi_clients

$(BUILD_FOLDER)/server: dgramInit.c
	$(CC) -DSERVER $^ -o $@

multi_clients: $(addprefix $(BUILD_FOLDER)/,client1 client2)

$(BUILD_FOLDER)/client1: dgramInit.c
	$(CC) -DCLIENT1 $^ -o $@

$(BUILD_FOLDER)/client2: dgramInit.c
	$(CC) -DCLIENT2 $^ -o $@

$(BUILD_FOLDER)/client: dgramInit.c
	$(CC) -DCLIENT $^ -o $@

$(BUILD_FOLDER)/client_response_no_bind: dgramInit.c
	$(CC) -DCLIENT_NO_BIND $^ -o $@

$(BUILD_FOLDER):
	mkdir -p build/

clean :
	rm build/*
