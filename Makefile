
CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -pedantic \
           -Wfloat-equal -Wshadow -Wno-unused-parameter -Wswitch-enum -Wcast-qual -Wnull-dereference -Wunused-result
INCLUDES = -Isrc/ -Ilib/
LIBS     = -lm
BIN      = bin/
TARGET   = main
SRC      = main.c $(wildcard src/*.c) $(wildcard lib/*.c)
OBJ      = $(addprefix $(BIN),$(SRC:.c=.o))

.PHONY: all clean

all: $(TARGET)

# debug-specific
debug: CFLAGS += -DDEBUG -ggdb -O0
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(BIN)%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BIN) $(TARGET)

