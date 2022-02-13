CC=gcc
CCX=g++
CFLAGS=-g3 -MMD -MP -DSPDLOG_FMT_EXTERNAL=ON -Wall -Lbuild $(INCLUDES) -fPIC
CXXFLAGS=${CFLAGS} -Wno-invalid-offsetof

LIBRARIES=-lBulletCollision -lLinearMath -lBulletDynamics -lglfw -lGL -ldl -lfmt
INCLUDES=-Isrc -I/usr/include/bullet -Iinclude

.PHONY: docs test

SRCFILES := $(shell find src/cel -type f -name "*.cpp")
HDRFILES := $(shell find src/cel -type f -name "*.h")
OBJS     := $(SRCFILES:.cpp=.o)
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

all: $(OBJS)
	${CCX} -fPIC -shared -o build/libcel.so $^ $(LIBRARIES) src/glad.c

-include $(DEPFILES)

example:
	${CXX} $(CXXFLAGS) $(LIBRARIES) $(INCLUDES) -lcel -o build/celtest src/celtest/celtest.cpp
docs:
	rm -rf docs
	doxygen docs.conf