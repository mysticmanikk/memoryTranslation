// phyframe.c
#include "memory.h"
#include <stdio.h>

void initialize_physical_frames(PhysicalFrames *pf) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        pf->frame_to_page[i] = -1; // -1 indicates the frame is free
        pf->last_used[i] = -1;     // For LRU, -1 means never used
    }
    pf->next_free_frame = 1; // Start allocating frames from 1 (0 is reserved for OS)
}

int allocate_frame(PhysicalFrames *pf, int page_num) {
    // If there's a free frame, allocate it
    if (pf->next_free_frame < NUM_FRAMES) {
        int frame = pf->next_free_frame++;
        pf->frame_to_page[frame] = page_num;
        return frame;
    } else {
        // No free frames; use LRU replacement
        return lru_replace(pf);
    }
}

void update_page_usage(PhysicalFrames *pf, int frame_num, int access_time) {
    pf->last_used[frame_num] = access_time; // Update last used time
}

int lru_replace(PhysicalFrames *pf) {
    // Find the least recently used frame
    int lru_frame = -1, oldest_time = __INT_MAX__;
    for (int i = 1; i < NUM_FRAMES; i++) { // Start from 1, skipping frame 0
        if (pf->last_used[i] < oldest_time) {
            oldest_time = pf->last_used[i];
            lru_frame = i;
        }
    }
    return lru_frame;
}
