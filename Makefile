CXX		?= g++
CXXFLAGS 	+= -Iinclude -ggdb3 -std=c++17 -fPIC
SRC		= $(wildcard src/*.cc src/toml++/*.cc)
OBJ		= $(SRC:.cc=.o)

UNAME_S := $(shell uname -s)

# is macos?
ifeq ($(UNAME_S),Darwin)
    LIBNAME      := libgithub-plugin.dylib
    CXXFLAGS     += -dynamiclib
    SONAME_FLAGS :=
else
    LIBNAME      := libgithub-plugin.so
    CXXFLAG      += -shared
    SONAME_FLAGS := -Wl,--export-dynamic
endif


all: libgithub-plugin

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

libgithub-plugin: $(OBJ)
	mkdir -p build
	$(CXX) $(OBJ) $(CXXFLAGS) -shared -o build/$(LIBNAME) -lcpr -lcufetch-fmt $(SONAME_FLAGS) -Wl,-undefined,dynamic_lookup

distclean:
	rm -rf $(OBJ)
	find . -type f -name "*.tar.gz" -delete
	find . -type f -name "*.o" -delete
	find . -type f -name "*.a" -delete

clean:
	rm -rf $(OBJ)

.PHONY: clean distclean all libgithub-plugin
