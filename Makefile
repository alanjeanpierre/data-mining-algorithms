CC = g++

CFLAGS = -std=c++11 -g -Wall -Wno-unused -Wno-sign-compare

SRC = $(wildcard *.cc)
OBJ = $(SRC:.cc=.o)

algos: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) algos