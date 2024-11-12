#include <stdio.h>
#include <stdint.h>

#define FRAMES 256  // physical frames

 int frames[FRAMES];  // Array representing physical frames (1 for allocated, 0 for free)
int free_frame_index = 1;  // Start allocating from frame 1 (frame 0 is reserved for OS)

void init_frames() {
    // Initialize frames to be free (value 0)
    for (int i = 0; i < FRAMES; i++) {
        frames[i] = 0;
    }
}

int allocate_frame() {
    // Allocate the next available frame
    if (free_frame_index < FRAMES) {
        frames[free_frame_index] = 1;  // Mark the frame as allocated
        return free_frame_index++;  // Return the frame and increment the index
    }
    return -1;  // No free frames available
}

void free_frame(int frame) {
    // Free the allocated frame
    if (frame >= 1 && frame < FRAMES) {
        frames[frame] = 0;  // Mark the frame as free
    }
}
