CC=gcc
CCX=g++
CFLAGS=-g3 -Wall -Wno-invalid-offsetof -Isrc -Iinclude -Lbuild -lglfw -lGL -ldl
CXXFLAGS=${CFLAGS}

CEL_SRC=src/glad.c \
src/cel/window.cpp \
src/cel/cel.cpp \
src/cel/io/binary_stream.cpp \
src/cel/framework/scene.cpp \
src/cel/framework/object.cpp \
src/cel/io/obj_importer.cpp \
src/cel/framework/components/component.cpp \
src/cel/reflect/reflect.cpp \
src/cel/render/render.cpp

CEL_TEST_SRC=src/celtest/celtest.cpp

all: test_project

cel:
	$(info Building Cel Library...)
	mkdir -p build
	${CXX} ${CXXFLAGS} -fPIC -shared -o build/libcel.so ${CEL_SRC}

test_project: cel assets
	$(info Building Test Project...)
	mkdir -p build	
	${CXX} ${CXXFLAGS} -lcel -o build/celtest ${CEL_TEST_SRC}

assets:
	$(info Building Test Project...)
	mkdir -p build
	$(info Clearing old shader directory...)
	rm -rf build/shaders
	$(info Copying new shader directory...)
	cp -r shaders build
