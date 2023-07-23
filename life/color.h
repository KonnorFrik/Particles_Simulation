#ifndef __COLORS_H__
#define __COLORS_H__

#include "life.h"

/*****COLOR MODULE*********/
const int COLOR_CODES[] = { RED, GREEN };
int* COLORS[sizeof(COLOR_CODES) / sizeof(COLOR_CODES[0])];
int RED_COLOR[4] = { 255, 0, 0, 255 };
int GREEN_COLOR[4] = { 0, 255, 0, 255 };
COLORS[RED] = RED_COLOR;
COLORS[GREEN] = GREEN_COLOR;
/*****COLOR MODULE*********/

#endif
