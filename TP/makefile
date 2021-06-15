CC=gcc -Wall
SERVER_DEFINE=-DSERVER
CLIENT_DEFINE=-DCLIENT
LIB_FOLDER=lib/
BUILD_FOLDER=build/

all : server client

server: $(LIB_FOLDER)libMCS.a StreamCltServ.c $(BUILD_FOLDER)
	$(CC) -o $(BUILD_FOLDER)$@ $< $(word 2, $^)

client: $(LIB_FOLDER)libMCS.a StreamCltServ.c $(BUILD_FOLDER)
	$(CC) -o $(BUILD_FOLDER)$@ $< $(word 2, $^)

$(LIB_FOLDER)libMCS.a: $(BUILD_FOLDER)proto.o $(BUILD_FOLDER)data.o $(BUILD_FOLDER)session.o $(LIB_FOLDER) $(BUILD_FOLDER)
	ar rcs $@ $(BUILD_FOLDER) $< $(word 2, $^) $(word 3, $^)

$(BUILD_FOLDER)proto.o: proto.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

$(BUILD_FOLDER)data.o: data.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

$(BUILD_FOLDER)session.o: session.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

$(LIB_FOLDER):
	mkdir -p $@

$(BUILD_FOLDER):
	mkdir -p $@

clean: clean_lib clean_build

clean_lib: $(LIB_FOLDER) $(BUILD_FOLDER)
	rm $<*; rm $(word 2, $^)/*.o

clean_build: $(BUILD_FOLDER)
	rm $^*
