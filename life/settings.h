#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define DEBUG 1
#define MESSAGES 1

#define FPS 40
#define MIN_FPS 1
#define MAX_FPS 60

#define WIN_H 1080
#define WIN_W 930
#define WINDOW_ZERO_POS 0

#define EXIT_CODE -1
enum ERR_CODES {
    NEUTRAL = 0,
    NULL_PTR,       // 1
    INPUT_ERR,      // 2
    INIT_ERR = 10,
};

enum RGBA_CODES {
    R = 0,
    G,
    B,
    A,
};

#endif
