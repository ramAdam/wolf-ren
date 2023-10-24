CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -I/usr/include/SDL2
LIBS = -lSDL2 -lm

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = $(BIN_DIR)/raycasting

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLUDE_DIR)

clean:
	rm -f $(TARGET) $(OBJS)
