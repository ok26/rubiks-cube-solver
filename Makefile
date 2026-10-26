# Thin wrapper around CMake so `make`, `make run`, `make tables` and `make clean` work from the project root

BUILD_DIR  ?= build
BUILD_TYPE ?= Release

.PHONY: all build run tables verify-tables clean

all: build

$(BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR) --parallel

run: build
	cd $(BUILD_DIR) && ./rubiks-cube-solver

tables: build
	cd $(BUILD_DIR) && ./generate-tables

verify-tables: build
	cd $(BUILD_DIR) && ./generate-tables verify

clean:
	rm -rf $(BUILD_DIR)
