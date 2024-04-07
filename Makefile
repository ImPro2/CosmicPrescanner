CC = gcc
CFLAGS = -g -Wall
TARGET = a.out
TARGET_DIR = bin
INT_DIR = bin-int

SRC_DIR = src
INC_DIR = include

INC_FLAGS = -I$(INC_DIR)

CPPFLAGS = -MMD -MP

SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%,$(INT_DIR)/%,$(SRC_FILES:.c=.o))
DEP_FILES = $(OBJS:.o=.d)

.PHONY: all run clean

all: $(TARGET_DIR)/$(TARGET)

run: all
	./bin/a.out

clean:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILES)
	rm -rf $(TARGET_DIR)
	rm -rf $(INT_DIR)

$(TARGET_DIR)/$(TARGET): $(TARGET_DIR) $(INT_DIR) $(OBJ_FILES)
	$(CC) -g $(OBJ_FILES) -o $@

$(TARGET_DIR):
	mkdir $@

$(INT_DIR):
	mkdir $@

$(INT_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC_FLAGS) -c -o $@ $<
