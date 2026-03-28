CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
TARGET = tiny-shell
SRC = src/main.c src/input.c src/tokenize.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)