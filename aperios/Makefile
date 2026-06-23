CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

APERIOS_INCLUDE := $(CURDIR)/include
HELLO_WORLD_SRC := ../samples/common/HelloWorld/HelloWorld/HelloWorld.cc
HELLO_WORLD_RUNNER := src/hello_world_lifecycle.cpp
HELLO_WORLD_INCLUDES := -I$(APERIOS_INCLUDE) -I../samples/common/HelloWorld/HelloWorld

.PHONY: all clean run-hello-world

all: hello-world-host

hello-world-host: $(HELLO_WORLD_SRC) $(HELLO_WORLD_RUNNER)
	$(CXX) $(CXXFLAGS) $(HELLO_WORLD_INCLUDES) $^ -o $@

run-hello-world: hello-world-host
	./hello-world-host

clean:
	rm -f hello-world-host
