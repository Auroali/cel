CC=gcc
CCX=g++
CFLAGS=-g3 -DSPDLOG_FMT_EXTERNAL=ON -Wall -Wno-invalid-offsetof -Lbuild ${LIBRARIES} ${INCLUDES}
CXXFLAGS=${CFLAGS}

LIBRARIES=-lBulletCollision -lLinearMath -lBulletDynamics -lglfw -lGL -ldl -lfmt
INCLUDES=-Isrc -I/usr/include/bullet -Iinclude

CEL_SRC=\
src/cel/reflect/reflect.cpp \
src/glad.c \
src/cel/window.cpp \
src/cel/cel.cpp \
src/cel/framework/scene.cpp \
src/cel/framework/object.cpp \
src/cel/framework/components/component.cpp \
src/cel/render/render.cpp \
src/cel/io/binary_stream.cpp \
src/cel/io/obj_importer.cpp \
src/cel/io/scene_io.cpp \
src/cel/physics/physics.cpp \

CEL_TEST_SRC=src/celtest/celtest.cpp

.PHONY: docs

all: cel test_project

cel:
	$(info Building Cel Library...)
	mkdir -p build
	${CXX} ${CXXFLAGS} -fPIC -shared -o build/libcel.so ${CEL_SRC}

test_project: assets
	$(info Building Test Project...)
	mkdir -p build
	$(info Clearing old shader directory...)
	rm -rf build/assets
	$(info Copying new shader directory...)
	cp -r assets build
	$(info Building Test Project...)
	mkdir -p build	
	${CXX} ${CXXFLAGS} -lcel -o build/celtest ${CEL_TEST_SRC}

docs:
	rm -rf docs
	doxygen docs.conf