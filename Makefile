CXX		?= g++
CXX_INCLUDES 	= -Iinclude
CXX_FLAGS 	= -shared -fPIC
SRC		= $(wildcard src/*.cc)
OBJ		= $(SRC:.cc=.o)

all: libgithub-plugin

%.o: %.cc
	$(CXX) $(CXX_INCLUDES) $(CXX_FLAGS) -c -o $@ $<

libgithub-plugin: $(OBJ)
	$(CXX) $(OBJ) $(CXX_INCLUDES) $(CXX_FLAGS) -o $@.so -lfmt -lcpr

clean:
	rm -f *.o *.so *.a

.PHONY: clean all libgithub-plugin
