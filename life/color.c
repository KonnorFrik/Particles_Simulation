#include <stdio.h>
#include <stdlib.h>

//#include "life.h"
#include "color.h"
//#include "settings.h"

int RED_COLOR[4] = {255, 0, 0, 255};
int GREEN_COLOR[4] = {0, 255, 0, 255};
int BLUE_COLOR[4] = {0, 0, 255, 255};
int YELLOW_COLOR[4] = {0, 255, 255, 255};
int WHITE_COLOR[4] = {255, 255, 255, 255};
int* ALL_PREDEF_COLORS[MAX_PREDEF_COLORS] = {RED_COLOR, GREEN_COLOR, BLUE_COLOR, YELLOW_COLOR, WHITE_COLOR};

int* get_color_by_id(int color_id) {
    int* res = NULL;

    if (color_id >= 0 && color_id < MAX_PREDEF_COLORS) {
        res = ALL_PREDEF_COLORS[color_id];

    } else {
        // full random colors
        res = calloc(4, sizeof(int));
        if (res != NULL) {
            for (int i = 0; i < 3; ++i) {
                res[i] = rand() % 255;
            }
            res[3] = 255;
        }
    }
    return res;
}
