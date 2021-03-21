SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

EXE := $(BIN_DIR)/rehoboam

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
COMPILED_OBJECTS := $(wildcard $(OBJ_DIR)/*.o)

CURRENT_DIRECTORY=$(shell pwd)

RGB_INCDIR=$(CURRENT_DIRECTORY)/include/rpi-led-matrix
RGB_LIBRARY_NAME=rgbmatrix

CPPFLAGS :=-Iinclude -I$(RGB_INCDIR) -I/opt/vc/include -MMD -MP
CXXFLAGS :=-O2 -W -Wall -Wextra -Wno-unused-parameter -D_FILE_OFFSET_BITS=64
LDFLAGS :=-Llib -L/opt/vc/lib
LDLIBS :=-l$(RGB_LIBRARY_NAME) -lm -lpthread -lrt -lbrcmEGL -lbrcmGLESv2 -lGLESv2

.PHONY: all clean

all:
	# +$(MAKE) -C src/Applications
	+$(MAKE) -C src/OpenGL
	+$(MAKE) -C src/Window
	+$(MAKE) $(EXE)

$(EXE): $(COMPILED_OBJECTS) $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(COMPILER) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command


-include $(OBJ:.o=.d)
