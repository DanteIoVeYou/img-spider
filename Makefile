CC=g++
FLAG=-std=c++11 -lpthread
BIN=unitTest.cpp
.PHONY:client
client: $(BIN)
	$(CC) -o $@ $^ $(FLAG)
.PHONY:clean
clean:
	rm -f client
.PHONY:debug
debug:
	$(CC) -o $@ $^ $(FLAG) -g
