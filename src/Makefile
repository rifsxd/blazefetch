# Makefile for blazefetch C++ project

# Source file
SRC = blazefetch.cpp

# Source file
SRC_TEST = blazefetch_test.cpp

# Binary file
BIN = blazefetch

# Binary file
BIN_TEST = blazefetch_test

# Base location
PREFIX ?= /usr/

# Flags
FLAG = -lX11
ICONIC = "-D ICONIC"

# Binary Folder
INSTALLPATH = ${DESTDIR}${PREFIX}/bin

# Compiler
CXX = g++

install : build

	@echo "installing binary file to ${INSTALLPATH}/${BIN}"
	@cp build/${BIN} ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/${BIN}
	@echo "installed"

install_iconic : build_iconic

	@echo "installing binary file to ${INSTALLPATH}/${BIN}"
	@cp build/${BIN} ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/${BIN}
	@echo "installed"

install_test : build_test

	@echo "installing binary file to ${INSTALLPATH}/${BIN_TEST}"
	@cp build/${BIN_TEST} ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/${BIN}
	@echo "installed"

install_test_iconic: build_test_iconic

	@echo "installing binary file to ${INSTALLPATH}/${BIN_TEST}"
	@cp build/${BIN_TEST} ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/${BIN}
	@echo "installed"


# Clean build files
clean:
	rm -rf build

# Build the project
build:
	mkdir build
	$(CXX) -o build/$(BIN) $(SRC) $(FLAG)

# Build the project with icons
build_iconic:
	mkdir build
	$(CXX) -o build/$(BIN) $(SRC) $(ICONIC) $(FLAG)

build_test:
	mkdir build
	$(CXX) -o build/$(BIN_TEST) $(SRC_TEST) $(FLAG)

build_test_iconic:
	mkdir build
	$(CXX) -o build/$(BIN_TEST) $(SRC_TEST) $(ICONIC) $(FLAG)

# Run the project
run: build
	./$(BIN)
