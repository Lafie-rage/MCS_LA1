CC=gcc #-Wall -Wextra
SERVER_DEFINE=-DSERVER
CLIENT_DEFINE=-DCLIENT
LIB_FOLDER=lib/
BUILD_FOLDER=build/
SRC_FOLDER=src/
READ_LIB_FOLDER=$(SRC_FOLDER)read_lib/

all : server client

server: $(SRC_FOLDER)StreamCltSrv.c $(LIB_FOLDER)libmcs.a $(LIB_FOLDER)libread.a
	$(CC) -DSERVER -o $@ $^

client: $(SRC_FOLDER)StreamCltSrv.c $(LIB_FOLDER)libmcs.a $(LIB_FOLDER)libread.a
	$(CC) -DCLIENT -o $@ $^

libs: $(LIB_FOLDER)libread.a $(LIB_FOLDER)libmcs.a

libread: $(LIB_FOLDER)libread.a

libmcs: $(LIB_FOLDER)libmcs.a

# Lib read
$(LIB_FOLDER)libread.a: $(BUILD_FOLDER)read_lib.o $(LIB_FOLDER) $(BUILD_FOLDER)
	ar rcs $@ $<

$(BUILD_FOLDER)read_lib.o: $(READ_LIB_FOLDER)read_lib.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

# Lib global MCS
$(LIB_FOLDER)libmcs.a: $(BUILD_FOLDER)proto.o $(BUILD_FOLDER)user.o $(BUILD_FOLDER)data.o $(BUILD_FOLDER)session.o $(LIB_FOLDER) $(BUILD_FOLDER)
	ar rcs $@ $< $(word 2, $^) $(word 3, $^) $(word 4, $^)

$(BUILD_FOLDER)proto.o: $(SRC_FOLDER)proto.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@
$(BUILD_FOLDER)user.o: $(SRC_FOLDER)user.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

$(BUILD_FOLDER)data.o: $(SRC_FOLDER)data.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

$(BUILD_FOLDER)session.o: $(SRC_FOLDER)session.c $(BUILD_FOLDER)
	$(CC) -c $< -o $@

$(LIB_FOLDER):
	mkdir -p $@

$(BUILD_FOLDER):
	mkdir -p $@

clean: clean_lib clean_exe

clean_lib: $(LIB_FOLDER) $(BUILD_FOLDER)
	rm $<* ; rm $(word 2, $^)*.o

clean_exe: 
	rm client ; rm server
