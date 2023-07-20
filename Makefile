.RECIPEPREFIX = >
.PHONY: all clean
# A simple Makefile for compiling small SDL projects

CC := clang
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99  -lSDL2_image -lm
W_FLAGS := -Wall -Wextra #-Werror
build_dir=bin

#HEADERS (what is it and for what?)
HDRS :=

all: check_dirs life clean

life: life.o atom_processing.o
> $(CC) $? $(CFLAGS) -o $(build_dir)/$@

check_dirs: check_dirs.sh
> chmod +x check_dirs.sh
> ./check_dirs.sh

#showfont: showfont.c Makefile
#> $(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

#glfont: glfont.c Makefile
#> $(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

clean:
> rm -f *.o

clean_all: clean
> rm -rf $(build_dir)/*

%.o: %.c
> $(CC) $(W_FLAGS) -c $? -o $@
