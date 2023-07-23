.RECIPEPREFIX = >
.PHONY: all
# A simple Makefile for compiling small SDL projects

CC := clang
SDL_FLAGS := `sdl2-config --libs --cflags` -lSDL2_image
CFLAGS := -ggdb3 -O0 --std=c99 -lm
W_FLAGS := -Wall -Wextra#-Werror
build_dir=bin
life_src_dir=life
utils_src_dir=utils

#HEADERS (what is it and for what?)
HDRS :=

all: check_dirs life rule_manager clean

life: life.o atom_processing.o my_math.o
> $(CC) $? $(SDL_FLAGS) $(CFLAGS) -o $(build_dir)/$@

rule_manager: rule_manager.o io.o
> $(CC) $? $(CFLAGS) -o $(build_dir)/$@

check_dirs: check_dirs.sh
> chmod +x check_dirs.sh
> ./check_dirs.sh

debug: $(life_src_dir)/life.c $(life_src_dir)/atom_processing.c $(life_src_dir)/my_math.c
> $(CC) $(CFLAGS) -g $? -o $(build_dir)/$@

clean:
> rm -f *.o

clean_all: clean
> rm -rf $(build_dir)/*

%.o: $(life_src_dir)/%.c
> $(CC) $(W_FLAGS) -c $? -o $@

%.o: $(utils_src_dir)/%.c
> $(CC) $(W_FLAGS) -c $? -o $@
