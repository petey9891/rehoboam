OBJ_DIR := ../../build
BIN_DIR := ../../bin


SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

CPPFLAGS :=-I../../include -I/opt/vc/include -MMD -MP
CXXFLAGS :=-O3 -W -Wall -Wextra -Wno-unused-parameter -D_FILE_OFFSET_BITS=64

.PHONY: applications

applications: $(OBJECTS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) # The @ disables the echoing of the command

-include $(OBJECTS:.o=.d)
