#include <stdio.h>
#include <stdint.h>

#define TOTAL_FRAMES 256  // Number of physical frames (change as necessary)

static int frames[TOTAL_FRAMES];  // Array to represent the physical frames.
static int free_frame_index = 1;  // Start allocation from frame 1.

void init_frames() {
    // Initialize all frames to be "free" (value 0).
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frames[i] = 0;
    }
}

int allocate_frame() {
    // Allocate the next available frame.
    if (free_frame_index < TOTAL_FRAMES) {
        frames[free_frame_index] = 1;  // Mark as allocated
        return free_frame_index++;
    }
    return -1;  // No free frames available
}

void free_frame(int frame) {
    // Free a specific physical frame.
    if (frame >= 1 && frame < TOTAL_FRAMES) {
        frames[frame] = 0;
    }
}
