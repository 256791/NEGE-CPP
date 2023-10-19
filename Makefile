ifeq ($(OS), Windows_NT)
	RMD = rmdir 2>nul /s /q
	MD = mkdir 2>nul
	DYNAMIC = dll
	FLEX = win_flex
else
ifeq ($(shell uname -s), Linux)
	OS = Linux
	RMD = rm -rf
	MD = mkdir -p
	DYNAMIC = so.3
	FLEX = flex
endif
endif

CC = g++
CPP_FLAGS = -std=c++17 -Wall -O3

EXAMPLE_DIR = examples
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
LIB_DIR = lib
OUT_DIR = bin

TARGET = nege
DEFINES = GLFW_DLL GLFW_INCLUDE_NONE

LIB := $(wildcard $(LIB_DIR)/*.$(DYNAMIC))
FLX := $(wildcard $(SRC_DIR)/*/*.ll)
FLC := $(FLX:%.ll=%.cpp)
SRC := $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*.cpp) $(FLC)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) $(OBJ_DIR)/glad.o
DEF := $(DEFINES:%=-D %)

EXAMPLES_SRC := $(wildcard $(EXAMPLE_DIR)/*.cpp)
EXAMPLES := $(EXAMPLES_SRC:$(EXAMPLE_DIR)/%.cpp=%)

OBJ_DIRS := $(sort $(dir $(wildcard $(SRC_DIR)/*/)))
OBJ_DIRS := $(OBJ_DIRS:$(SRC_DIR)/%="$(OBJ_DIR)/%")

all: examples

debug: DEF := $(DEF) -D DEBUG
debug: examples

release: DEF := $(DEF) -D RELEASE
release: examples

examples: $(EXAMPLES)

render: render/HolonomyTopView.mp4 render/Holonomy.mp4 render/Distortion.mp4 render/Space_E3.mp4 render/Space_H3.mp4

render/%.mp4: %
	@$(RMD) "render/$<" ||:
	@$(MD) "render/$</frames"
	@$(MD) "render/$</keyframes"
	./bin/$< save
	ffmpeg -y -i 'render/$</frames/capture_%d.png' -framerate 60 -r 60 -c:v libx264 -pix_fmt yuv420p '$@'
	ffmpeg -y -i $@ -filter_complex "[0:v] fps=10,scale=640:-1,split [a][b];[a] palettegen [p];[b][p] paletteuse" "render/$<.gif"

$(EXAMPLES): $(OBJ)
	@$(MD) "$(OUT_DIR)" ||:
	$(CC) $(CPP_FLAGS) -I $(INC_DIR) -I $(SRC_DIR) -o $(OUT_DIR)/$@ $^ $(EXAMPLE_DIR)/$@.cpp $(LIB)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MD) "$(@D)" ||:
	$(CC) $(CPP_FLAGS) $(DEF) -I $(INC_DIR) -c $< -o $@

$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c
	@$(MD) "$(@D)" ||:
	$(CC) $(CPP_FLAGS) $(DEF) -I $(INC_DIR) -c $< -o $@

$(SRC_DIR)/%Lexer.cpp: $(SRC_DIR)/%Lexer.ll
	@$(MD) "$(@D)" ||:
	$(FLEX) -+ -o $@ $<

# doc:
# 	doxygen dconfig

clean:
	@$(RMD) $(OBJ_DIR) ||:
	@$(RMD) $(OUT_DIR) ||:
	@$(RMD) "render" ||:



# ffmpeg -y -i "render/Holonomy.gif" -i "render/HolonomyTopView.gif" -filter_complex hstack "img/HolonomyStack.gif"
# ffmpeg -y -i "render/Space_H3.gif" -i "render/Space_E3.gif" -filter_complex hstack "img/SpaceStack.gif"
