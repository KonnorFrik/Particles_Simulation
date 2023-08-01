#ifndef __COLORS_H__
#define __COLORS_H__

#define MAX_PREDEF_COLORS 5

const int RED_COLOR[4] = { 255, 0, 0, 255 };
const int GREEN_COLOR[4] = { 0, 255, 0, 255 };
const int BLUE_COLOR[4] = { 0, 0, 255, 255 };
const int YELLOW_COLOR[4] = { 0, 255, 255, 255 };
const int WHITE_COLOR[4] = { 255, 255, 255, 255 };
const int* ALL_PREDEF_COLORS[MAX_PREDEF_COLORS] = { RED_COLOR, GREEN_COLOR, BLUE_COLOR, YELLOW_COLOR, WHITE_COLOR };

void fill_colors(int** colors, int curr_len, int max_len);

#endif
