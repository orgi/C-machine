
test-sources=$(wildcard test/*.cpp)
test-objects=$(test-sources:%.cpp=%.o)

sources:=$(wildcard *.cpp)
sources:=$(filter-out C-machine.cpp,$(sources))
objects=$(sources:%.cpp=%.o)


CC=g++

all: C-machine

test: C-machine-test
	./C-machine-test

C-machine: $(objects)
	$(CC) -o $@ $^

C-machine-test: $(test-objects) $(objects)
	$(CC) -o $@ $^ -l

clean:
	rm -f C-machine C-machine-test