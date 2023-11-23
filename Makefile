.RECIPEPREFIX = >
.PHONY: all
# A simple Makefile for compiling small SDL projects

CC := gcc
#SDL_FLAGS := `sdl2-config --libs --cflags` -lSDL2_image
SDL_FLAGS := `sdl2-config --libs --cflags` -lSDL2_image
CFLAGS := -ggdb3 -O0 --std=c11 -lm -fPIE
W_FLAGS := -Wall -Wextra -Werror
build_dir=bin
life_src_dir=life
utils_src_dir=utils
obj_dir=obj_files

#HEADERS (what is it and for what?)
HDRS :=

all: check_dirs life rule_manager clean

life: $(obj_dir)/life.o $(obj_dir)/atom_processing.o $(obj_dir)/my_math.o $(obj_dir)/io.o $(obj_dir)/color.o
> $(CC) $? $(SDL_FLAGS) $(CFLAGS) -o $(build_dir)/$@

rule_manager: $(obj_dir)/rule_manager.o $(obj_dir)/read_rule.o $(obj_dir)/write_rule.o $(obj_dir)/io.o
> $(CC) $? $(CFLAGS) -o $(build_dir)/$@

check_dirs: check_dirs.sh
> chmod u+x check_dirs.sh
> ./check_dirs.sh

debug: $(life_src_dir)/life.c $(life_src_dir)/atom_processing.c $(life_src_dir)/my_math.c
> $(CC) $(CFLAGS) -g $? -o $(build_dir)/$@

rebuild: clean_all all

clean:
> rm -f $(obj_dir)/*.o

clean_all: clean
> rm -f $(build_dir)/*

$(obj_dir)/%.o: $(life_src_dir)/%.c
> $(CC) $(W_FLAGS) -c $? -o $@

$(obj_dir)/%.o: $(utils_src_dir)/%.c
> $(CC) $(W_FLAGS) -c $? -o $@

$(obj_dir)/%.o: %.c
> $(CC) $(W_FLAGS) -c $? -o $@
