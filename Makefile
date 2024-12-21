CC = gcc
CFLAGS = -g -Wall
EXEC = programa
OBJ = main.o

all: programa

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(EXEC) $(OBJ)