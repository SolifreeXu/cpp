IGNORE := .
ROOT := ..
SOURCE := $(ROOT)/Source
INCLUDE := $(ROOT)/Source
BINARY := $(ROOT)/Binary

CXXFLAGS := -std=c++17 -I$(INCLUDE)

TARGET := $(BINARY)/test

OBJECTS :=
OBJECTS += Integer.o
OBJECTS += test.o

default: $(OBJECTS)
	${CXX} $^ -o $(TARGET)
%.o: %.cpp
	${CXX} $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
