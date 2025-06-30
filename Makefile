CXX		?= g++
CXX_INCLUDES 	= -Iinclude
CXX_FLAGS 	= -ggdb3 -shared -fPIC
SRC		= $(wildcard src/*.cc)
OBJ		= $(SRC:.cc=.o)

all: libgithub-plugin

%.o: %.cc
	$(CXX) $(CXX_INCLUDES) $(CXX_FLAGS) -c -o $@ $<

libgithub-plugin: $(OBJ)
	$(CXX) $(OBJ) $(CXX_INCLUDES) $(CXX_FLAGS) -o $@.so -lcpr -lcufetch-fmt

distclean:
	rm -rf $(OBJ)
	find . -type f -name "*.tar.gz" -delete
	find . -type f -name "*.o" -delete
	find . -type f -name "*.a" -delete

clean:
	rm -rf $(OBJ)

.PHONY: clean distclean all libgithub-plugin
