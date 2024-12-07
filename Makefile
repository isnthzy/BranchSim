BUILD_DIR = ./build
-include config.mk
$(shell mkdir -p $(BUILD_DIR))

GCC = g++
INC_PATH = 
CXXFLAGS = -Wall -Werror -MMD -lstdc++
# CXXFLAGS += -O3
CXXFLAGS += -Og -g
INCFLAGS = $(addprefix -I, $(INC_PATH))
BIN = $(BUILD_DIR)/BranchSim
CSRCS = $(shell find $(abspath ./src) -name "*.c" -or -name "*.cpp" -or -name "*.cc")
TRACE = 

build:
	$(GCC) $(CXXFLAGS) $(CSRCS) -o $(BIN)

run: build
	$(BIN) $(ARGS) --btrace=$(TRACE)

gdb: build
	gdb -ex "set args $(ARGS) --btrace=$(TRACE)" $(BIN)
clean:
	rm -rf $(BUILD_DIR)

.PHONY: build run