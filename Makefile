CC=gcc
CCX=g++
MODULE_FLAGS=-D ENABLE_RENDERER -D ENABLE_IO -D ENABLE_PHYSICS
CFLAGS=-g3 ${MODULE_FLAGS} -Wall -Wno-invalid-offsetof -Isrc -I/usr/include/bullet -Iinclude -Lbuild -lglfw -lGL -ldl
CXXFLAGS=${CFLAGS}

CEL_SRC=\
src/glad.c \
src/cel/window.cpp \
src/cel/cel.cpp \
src/cel/framework/scene.cpp \
src/cel/framework/object.cpp \
src/cel/framework/components/component.cpp \
src/cel/reflect/reflect.cpp \


CEL_PHYSICS_MODULE=src/cel/physics/physics.cpp
CEL_RENDER_MODULE=src/cel/render/render.cpp
CEL_IO_MODULE=\
src/cel/io/binary_stream.cpp \
src/cel/io/obj_importer.cpp

CEL_TEST_SRC=src/celtest/celtest.cpp

.PHONY: docs

all: test_project

cel:
	$(info Building Cel Library...)
	mkdir -p build
	${CXX} ${CXXFLAGS} -fPIC -shared -o build/libcel.so ${CEL_SRC}
	${CXX} ${CXXFLAGS} -fPIC -shared -o build/libcelphys.so ${CEL_PHYSICS_MODULE}
	${CXX} ${CXXFLAGS} -fPIC -shared -o build/libcelrender.so ${CEL_RENDER_MODULE}
	${CXX} ${CXXFLAGS} -lBulletCollision -lLinearMath -lBulletDynamics -fPIC -shared -o build/libcelio.so ${CEL_IO_MODULE}

test_project: cel assets
	$(info Building Test Project...)
	mkdir -p build
	$(info Clearing old shader directory...)
	rm -rf build/assets
	$(info Copying new shader directory...)
	cp -r assets build
	$(info Building Test Project...)
	mkdir -p build	
	${CXX} ${CXXFLAGS} -lcelrender -lcelio -lcelphys -lcel -o build/celtest ${CEL_TEST_SRC}

docs:
	rm -rf docs
	doxygen docs.conf