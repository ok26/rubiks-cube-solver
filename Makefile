# Thin wrapper around CMake so `make`, `make run` and `make clean` work from the project root

BUILD_DIR  ?= build
BUILD_TYPE ?= Release

.PHONY: all build run clean

all: build

$(BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR) --parallel

run: build
	cd $(BUILD_DIR) && ./rubiks-cube-solver

clean:
	rm -rf $(BUILD_DIR)
