CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I/usr/include/SDL2
LIBS = -lSDL2 -lm

all: raycasting

raycasting: raycasting.o
	$(CC) $(CFLAGS) -o raycasting raycasting.o $(LIBS)

raycasting.o: raycasting.c
	$(CC) $(CFLAGS) -c raycasting.c

clean:
	rm -f raycasting raycasting.o
