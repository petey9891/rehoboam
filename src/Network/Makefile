OBJ_DIR := ../../build
BIN_DIR := ../../bin


SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

CPPFLAGS :=-I../../include -I../../../libraries/SocketServer/include -I/opt/vc/include -MMD -MP
CXXFLAGS :=-std=c++17 -O3 -W -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wno-psabi -D_FILE_OFFSET_BITS=64

.PHONY: server

server: $(OBJECTS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command

-include $(OBJECTS:.o=.d)
