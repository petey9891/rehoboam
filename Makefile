SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

EXE := $(BIN_DIR)/rehoboam

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
COMPILED_OBJECTS := $(wildcard $(OBJ_DIR)/*.o)

CURRENT_DIRECTORY=$(shell pwd)

RGB_INCDIR=$(CURRENT_DIRECTORY)/include/rpi-led-matrix

CPPFLAGS :=-Iinclude -I/opt/vc/include -MMD -MP
CXXFLAGS :=-std=c++17 -O3 -W -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wno-psabi -D_FILE_OFFSET_BITS=64
LDFLAGS :=-Llib -L/opt/vc/lib
LDLIBS :=-lssl -ldl -lcrypto -lrgbmatrix -lm -lpthread -lrt -lbrcmEGL -lbrcmGLESv2 -lGLESv2

.PHONY: all clean

all:
	+$(MAKE) -C src/Applications
	+$(MAKE) -C src/OpenGL
	+$(MAKE) -C src/Window
	+$(MAKE) -C src/Network
	+make $(EXE)

$(EXE): $(COMPILED_OBJECTS) $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command


-include $(OBJECTS:.o=.d)
