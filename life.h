#ifndef __LIFE_H__
#define __LIFE_H__

#include <SDL2/SDL.h>
#define MIN_ATOM_RADIUS 1
#define START_ATOM_RADIUS 5
#define MAX_ATOM_RADIUS 100

#define START_ATOMS_COUNT 100 // per group

#define POWER_MAX 4

enum color_codes {
    RED = 0,
    GREEN,
};

typedef struct {
    SDL_Rect* atom; // x y w h 
    int color_code;
    float* powers;
} ATOM;

void change_radius(SDL_Rect* rect, int radius);
ATOM* init_atom(int* status);
void fill_atom(ATOM* obj, int radius, int color, float* powers);
void destroy_atom(ATOM* obj);
void random_position(ATOM** group, int group_size);
float* get_random_powers(int groups);
void process_groups(ATOM*** all, int color1, int color2, int group_size);
void keep_in_screen(ATOM*** all, int groups, int atoms_per_group, int win_w, int win_h);

float frand();

#endif
