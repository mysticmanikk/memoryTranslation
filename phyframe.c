#include <stdio.h>
#include "phyframe.h"

#define FRAMES 8

int frames[FRAMES] = {0};
int next_free_frame = 1;

void init_frames() {
    for (int i = 1; i < FRAMES; i++) {
        frames[i] = 0;
    }
}

int allocate_frame() {
    if (next_free_frame < FRAMES) {
        frames[next_free_frame] = 1;
        return next_free_frame++;
    }
    return -1;
}

void free_frame(int frame) {
    if (frame > 0 && frame < FRAMES) {
        frames[frame] = 0;
    }
}
