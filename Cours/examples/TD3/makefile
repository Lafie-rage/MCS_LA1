CC=gcc
BUILD_FOLDER := build

all : $(addprefix $(BUILD_FOLDER)/,server client)

$(BUILD_FOLDER)/server: streamInit.c
	$(CC) -DSERVER $^ -o $@

$(BUILD_FOLDER)/client: streamInit.c
	$(CC) -DCLIENT $^ -o $@

$(BUILD_FOLDER):
	mkdir -p build/

clean :
	rm build/*
