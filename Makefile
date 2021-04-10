CC := cc 
BIN_DIR := ./bin
OBJ_DIR := $(BIN_DIR)/obj
TARGET := $(BIN_DIR)/tic

SRC := $(wildcard src/*.c)

OBJECTS = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $< -o $@ -Iinclude -c -g

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) -o $(TARGET) $^

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
