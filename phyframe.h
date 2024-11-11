// phyframe.h

#ifndef PHYFRAME_H
#define PHYFRAME_H

#define TOTAL_FRAMES 256  // Total number of physical frames

// Function to initialize physical frames (mark all frames as free).
void init_frames();

// Function to allocate a new physical frame and return the frame index.
int allocate_frame();

// Function to free a specific physical frame.
void free_frame(int frame);

#endif // PHYFRAME_H
