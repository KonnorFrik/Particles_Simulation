#include <stdlib.h>
#include <math.h>

#include "life.h"

float frand() {
    float a = POWER_MAX;
    float res = ((float)rand()/(float)(RAND_MAX)) * a;
    if ((rand() % 2) == 0) {
        res = -res;
    }

    if (fabs(res) < 1) {
        res < 0 ? res-- : res++;
    }

    return res;
}

