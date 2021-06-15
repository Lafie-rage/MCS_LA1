CC=gcc -Wall
SERVER_DEFINE=-DSERVER
CLIENT_DEFINE=-DCLIENT
LIB_FOLDER=lib/
BUILD_FOLDER=build/
READ_LIB_FOLDER=read_lib/

all : server client

server: StreamCltSrv.c $(LIB_FOLDER)libmcs.a $(LIB_FOLDER)libread.a $(BUILD_FOLDER)
	$(CC) -o $(BUILD_FOLDER)$@ $< $(word 2, $^) $(word 3, $^)

client: StreamCltSrv.c $(LIB_FOLDER)libmcs.a $(LIB_FOLDER)libread.a $(BUILD_FOLDER)
	$(CC) -o $(BUILD_FOLDER)$@ $< $(word 2, $^) $(word 3, $^)

# Lib read
$(LIB_FOLDER)libread.a: $(BUILD_FOLDER)read_lib.o $(BUILD_FOLDER)
	ar rcs $@ $<

$(BUILD_FOLDER)read_lib.o: $(READ_LIB_FOLDER)read_lib.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

# Lib global MCS
$(LIB_FOLDER)libmcs.a: $(BUILD_FOLDER)proto.o $(BUILD_FOLDER)data.o $(BUILD_FOLDER)session.o $(LIB_FOLDER) $(BUILD_FOLDER)
	ar rcs $@ $< $(word 2, $^) $(word 3, $^)

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
	rm $<* ; rm $(word 2, $^)*.o

clean_build: $(BUILD_FOLDER)
	rm $^*
