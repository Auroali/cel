CC=gcc
CCX=g++
CFLAGS=-Isrc -Iinclude -Lbuild -lglfw -lGL -ldl
CXXFLAGS=${CFLAGS}

CEL_SRC=src/glad.c \
src/cel/window.cpp \
src/cel/cel.cpp \
src/cel/io/binary_stream.cpp \
src/cel/io/shader.cpp

CEL_TEST_SRC=src/celtest/celtest.cpp

build:
	$(build_cel)
	$(build_testproj)

build_cel:
	mkdir -p build
	${CXX} ${CXXFLAGS} -fPIC -shared -o build/libcel.so ${CEL_SRC}

build_testproj:
	mkdir -p build
	rm -rf build/shaders
	cp -r shaders build
	${CXX} ${CXXFLAGS} -lcel -o build/celtest ${CEL_TEST_SRC}