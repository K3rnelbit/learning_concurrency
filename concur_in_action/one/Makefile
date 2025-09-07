
# Directory structure assumption:
# - src/: Source files (*.cpp)
# - include/: Header files (*.hpp or *.h)
# - build/: Object files and dependencies
# - bin/: Executables
# - lib/: Generated libraries (and optionally external libraries)
# - version.map: Version script for shared library symbol exports
#
# Usage:
# - make: Build release executable
# - make debug: Build debug executable
# - make static: Build fully static executable
# - make lib: Build static library
# - make shared: Build shared library with version script
# - make multilib: Placeholder for linking multiple libraries
# - make all: Build executable + libraries
# - make clean: Remove build artifacts (preserves external libraries and version.map)
# - make valgrind: Run debug executable under Valgrind
# - make run: Run the release executable
# - make bear: Generate compile_commands.json (incremental)
# - make bear-full: Generate compile_commands.json (full rebuild)

# Project settings
PROJECT_NAME 			:= project
EXECUTABLE 				:= $(PROJECT_NAME)
STATIC_LIB 				:= lib$(PROJECT_NAME).a
SHARED_LIB 				:= lib$(PROJECT_NAME).so
VERSION_SCRIPT 		:= version.map

# Directories 
SRC_DIR 					:= src
INCLUDE_DIR 			:= include
BUILD_DIR 				:= build
BIN_DIR 					:= bin
LIB_DIR 					:= lib

# Compiler and linker flags
CXX 									:= g++
CXXFLAGS 							:= -Wall -Wextra -Werror -Wpedantic -std=c++23 -I$(INCLUDE_DIR)
SHARED_FLAGS 					:= -fPIC
DEBUG_FLAGS 					:= -g -O0 -DDEBUG
RELEASE_FLAGS 				:= -O3 -DNDEBUG
STATIC_LDFLAGS 				:= -static
LDFLAGS 							:= -Wl,-rpath,$(LIB_DIR) # Add any general linker flags, e.g., -lpthread
SHARED_LDFLAGS 				:= -Wl,-soname $(SHARED_LIB) -Wl,--version-script,$(VERSION_SCRIPT) # Linker options for shared library

SOURCES 							:= $(wildcard $(SRC_DIR)/*.cpp) #find all source files
OBJECTS 							:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES)) #.o files for static lib/exec
SHARED_OBJECTS 				:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%-pic.o, $(SOURCES)) #shared lib with -fPIC

# Dependency files
DEPS := $(OBJECTS:.o=.d) $(SHARED_OBJECTS:.o=.d)

# Default target: release executable
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(BIN_DIR)/$(EXECUTABLE)

# Debug target
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(BIN_DIR)/$(EXECUTABLE)

# Static executable
static: CXXFLAGS += $(RELEASE_FLAGS)
static: LDFLAGS += $(STATIC_LDFLAGS)
static: $(LIB_DIR)/$(STATIC_LIB) $(BIN_DIR)/$(EXECUTABLE)-static

# Static library
lib: $(LIB_DIR)/$(STATIC_LIB)

# Shared library
shared: $(VERSION_SCRIPT) $(LIB_DIR)/$(SHARED_LIB)

# Placeholder for linking multiple libraries
multilib:
	@echo "Placeholder for linking multiple static/shared libraries."
# Create version script for shared library
$(VERSION_SCRIPT):
	@echo "Creating version script $(VERSION_SCRIPT)"
	@echo "MYPROJECT_1.0 {\n    global:\n        newFunction;\n    local:\n        *;\n};" > $(VERSION_SCRIPT)
# All: executable + libraries
all: release lib shared static

# Run the executable
run: release
	$(BIN_DIR)/$(EXECUTABLE)

# Valgrind (requires debug build)
valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN_DIR)/$(EXECUTABLE)

# Generate compile_commands.json (incremental)
bear:
	bear -- make debug

# Generate compile_commands.json (full rebuild)
bear-full: clean
	bear -- make debug

# Clean (only removes generated libraries, not external ones or version.map)
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)/$(STATIC_LIB) $(LIB_DIR)/$(SHARED_LIB) compile_commands.json

# Install
PREFIX ?= /usr/local
INSTALL_BIN_DIR = $(PREFIX)/bin
INSTALL_LIB_DIR = $(PREFIX)/lib

install: release lib shared
	install -d $(INSTALL_BIN_DIR) $(INSTALL_LIB_DIR)
	install -m 755 $(BIN_DIR)/$(EXECUTABLE) $(INSTALL_BIN_DIR)
	install -m 644 $(LIB_DIR)/$(STATIC_LIB) $(INSTALL_LIB_DIR)
	install -m 644 $(LIB_DIR)/$(SHARED_LIB) $(INSTALL_LIB_DIR)

# Help
help:
	@echo "Available targets:"
	@echo "  release      - Build release executable"
	@echo "  debug        - Build debug executable"
	@echo "  static       - Build static executable and library"
	@echo "  lib          - Build static library"
	@echo "  shared       - Build shared library"
	@echo "  all          - Build release, lib, shared, static"
	@echo "  run          - Build and run release executable"
	@echo "  valgrind     - Run debug executable with Valgrind"
	@echo "  bear         - Generate compile_commands.json"
	@echo "  clean        - Remove build artifacts"
	@echo "  install      - Install binaries and libraries to $(PREFIX)"

# Create directories if needed
$(BUILD_DIR) $(BIN_DIR) $(LIB_DIR):
	mkdir -p $@

# Compile object files (for executable and static lib), -MMD and -MP are for dependency generation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile object files for shared lib (with -fPIC)
$(BUILD_DIR)/%-pic.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) -MMD -MP -c $< -o $@

# Link executable
$(BIN_DIR)/$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

# Link static executable
$(BIN_DIR)/$(EXECUTABLE)-static: $(OBJECTS) $(LIB_DIR)/(STATIC_LIB) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -L$(LIB_DIR) -l$(PROJECT_NAME) -o $@

# Build static library
$(LIB_DIR)/$(STATIC_LIB): $(OBJECTS) | $(LIB_DIR)
	ar rcs $@ $(OBJECTS)

# Build shared library
$(LIB_DIR)/$(SHARED_LIB): $(SHARED_OBJECTS) | $(LIB_DIR)
	$(CXX) -shared $(SHARED_OBJECTS) $(LDFLAGS) $(SHARED_LDFLAGS) -o $@

# Include dependency files
-include $(DEPS)

.PHONY: all release debug static lib shared multilib clean run valgrind bear bear-full
