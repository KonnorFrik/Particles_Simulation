#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "life.h"
#include "settings.h"

void change_radius(SDL_Rect* rect, int radius) {
    rect->w = radius;
    rect->h = radius;

    if (rect->w <= MIN_ATOM_RADIUS || rect->h <= MIN_ATOM_RADIUS) {
        rect->w = MIN_ATOM_RADIUS;
        rect->h = MIN_ATOM_RADIUS;
    }

    if (rect->w >= MAX_ATOM_RADIUS || rect->h >= MAX_ATOM_RADIUS) {
        rect->w = MAX_ATOM_RADIUS;
        rect->h = MAX_ATOM_RADIUS;
    }

    if (DEBUG) {
        printf("[DEBUG] New radius = '%d'\n", rect->w);
    }
}

ATOM* init_atom(int* status) {
    SDL_Rect* atom_rect = calloc(1, sizeof(SDL_Rect));

    if (atom_rect == NULL) {
        *status = NULL_PTR;
    }

    ATOM* obj = calloc(1, sizeof(ATOM));
    if (obj != NULL) {
        obj->atom = atom_rect;
        obj->color_code = 0;
        obj->powers = NULL;
        obj->vx = 0;
        obj->vy = 0;
        obj->search_radius = ATOM_SEARCH_RADIUS;

    } else {
        *status = NULL_PTR;

        if (atom_rect != NULL) {
            free(atom_rect);
        }
    }

    return obj;
}

void fill_atom(ATOM* obj, int radius, int color, float* powers) {
    if (obj != NULL) {
        if (obj->atom != NULL) {
            obj->atom->w = radius;
            obj->atom->h = radius;
        }
        obj->color_code = color;
        obj->powers = powers;
    }
}

void destroy_atom(ATOM* obj) {
    if (obj != NULL) {
        // if (obj->atom != NULL) {
        // free(obj->atom);
        //}

        // if (obj->powers != NULL) {
        // free(obj->powers);
        //}
        free(obj);
    }
}

void random_position(ATOM** group, int group_size, int win_w, int win_h) {
    for (int i = 0; i < group_size; ++i) {
        group[i]->atom->x = rand() % win_w;
        group[i]->atom->y = rand() % win_h;
    }
}

float* get_random_powers(int groups) {
    float* pwrs = calloc(groups, sizeof(float));
    if (pwrs != NULL) {
        for (int i = 0; i < groups; ++i) {
            pwrs[i] = frand();
        }
    }

    return pwrs;
}

/*
float read_powers_file(int color) {
}
 */

void process_groups(ATOM*** all, int color1, int color2, int group_size) {
    ATOM** group1 = all[color1];
    ATOM** group2 = all[color2];
    for (int first = 0; first < group_size; ++first) {
        for (int second = 0; second < group_size; ++second) {
            float fx = 0;
            float fy = 0;

            float dx = group1[first]->atom->x - group2[second]->atom->x;
            float dy = group1[first]->atom->y - group2[second]->atom->y;
            int d = sqrt((dx * dx) + (dy * dy));
            if (d > 0 && d < group1[first]->search_radius) {
                float F = group1[first]->powers[color2] * 1 / d;
                fx += F * dx;
                fy += F * dy;

                // if (DEBUG) {
                // printf("F: %f\n", F);
                //}
            }

            group1[first]->vx = (group1[first]->vx + fx) * 0.5;
            group1[first]->vy = (group1[first]->vy + fy) * 0.5;

            if (fabs(group1[first]->vx) >= SPEED_LIMIT) {
                if (group1[first]->vx < 0) {
                    group1[first]->vx = -SPEED_LIMIT;

                } else {
                    group1[first]->vx = SPEED_LIMIT;
                }
            }

            if (fabs(group1[first]->vy) >= SPEED_LIMIT) {
                if (group1[first]->vy < 0) {
                    group1[first]->vy = -SPEED_LIMIT;

                } else {
                    group1[first]->vy = SPEED_LIMIT;
                }
            }

            group1[first]->atom->x += round(group1[first]->vx);
            group1[first]->atom->y += round(group1[first]->vy);

            // if (DEBUG) {
            // printf("[DEBUG]\n");
            // printf("f: %d, s:%d\n", first, second);
            // printf("fx: %f  fy: %f\n", fx, fy);
            // printf("dx: %f  dy: %f\n", dx, dy);
            // printf("vx: %f , vy: %f\n", group1[first]->vx, group1[first]->vy);
            // printf("d: %d\n", d);
            // printf("i: %d, atom[f]->x: %d  atom[f]->y: %d\n", first, group1[first]->atom->x,
            // group1[first]->atom->y); printf("\n");
            //}
        }
    }
}

void keep_in_screen(ATOM*** all, int groups, int atoms_per_group, int win_w, int win_h) {
    for (int gr = 0; gr < groups; ++gr) {
        for (int i = 0; i < atoms_per_group; ++i) {
            if ((all[gr][i]->atom->x + all[gr][i]->atom->w) >= win_w) {
                all[gr][i]->atom->x = win_w - all[gr][i]->atom->w;
                all[gr][i]->vx *= -1;

            } else if (all[gr][i]->atom->x <= 0) {
                all[gr][i]->atom->x = 1;
                all[gr][i]->vx *= -1;
            }

            if ((all[gr][i]->atom->y + all[gr][i]->atom->h) >= win_h) {
                all[gr][i]->atom->y = win_h - all[gr][i]->atom->w;
                all[gr][i]->vy *= -1;

            } else if (all[gr][i]->atom->y <= 0) {
                all[gr][i]->atom->y = 1;
                all[gr][i]->vy *= -1;
            }
        }
    }
}
