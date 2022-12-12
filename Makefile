ifeq ($(OS),Windows_NT)
	RMD = rmdir /s /q
	DYNAMIC = dll
else
    UNAME_S := $(shell uname -s)
		ifeq ($(UNAME_S),Linux)
			OS = Linux
			RMD = rm -r
			DYNAMIC = so
    endif
endif

CC = g++
CPP_FLAGS = -std=c++17 -Wall -O3

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
LIB_DIR = lib
OUT_DIR = bin

TARGET = nege
DEFINES = DEBUG GLFW_DLL

LIB := $(wildcard $(LIB_DIR)/*.$(DYNAMIC))
SRC := $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) $(OBJ_DIR)/glad.o
DEF := $(DEFINES:%=-D %)

OBJ_DIRS := $(sort $(dir $(wildcard $(SRC_DIR)/*/)))
OBJ_DIRS := $(OBJ_DIRS:$(SRC_DIR)/%="$(OBJ_DIR)/%")

all: $(TARGET)

release: DEF := $(DEF) -D RELEASE
release: $(TARGET)

run: $(TARGET)
	./bin/nege

$(TARGET): $(OBJ)
	$(CC) $(CPP_FLAGS) -o $(OUT_DIR)/$@ $^ $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPP_FLAGS) $(DEF) -I $(INC_DIR) -c $< -o $@

$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c
	$(CC) $(CPP_FLAGS) $(DEF) -I $(INC_DIR) -c $< -o $@


clean:
	$(RMD) $(OBJ_DIR)
	mkdir $(OBJ_DIRS)

info: ;
	@echo src: $(SRC)
	@echo obj: $(OBJ)