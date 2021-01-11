# Makefile
SHELL := /bin/bash
P="$(PWD)"
COMPILE_FILENAME?="main"
SOURCES=$(wildcard src/*.cpp)
HEADERS=$(wildcard includes/*.hpp)
INCLUDES="$P/includes"

main:	info clean
	echo "> Compiling production code for ${COMPILE_FILENAME}..."
	g++ -std=c++17 -o ${COMPILE_FILENAME} -O2 -I$(INCLUDES) $(SOURCES)
	echo "Compiled"

info:
	echo "> Current directory: $P"
	echo "> Compile file: $(HEADERS)"

clean:
	echo "> Cleaning previous compilation"
	@if [[ -e "$P/$COMPILE_FILENAME" ]]; then \
		echo "  Cleaning executables"; \
		rm -f ${COMPILE_FILENAME}; \
	fi
	echo "Cleaned"



debug:
	echo "Compiling project"

.SILENT:
