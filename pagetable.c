#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>  // Include limits.h for INT_MAX
#include "phyframe.h"

#define TOTAL_PAGES 128  // Number of virtual pages (can be adjusted)

typedef struct {
    int frame;  // Physical frame mapped to this virtual page
    int last_used;  // Timestamp for LRU
    int valid;  // Flag to indicate if the page is valid
} PageTableEntry;

 PageTableEntry page_table[TOTAL_PAGES];
 int reverse_mapping[TOTAL_FRAMES];  // Reverse mapping from frame to virtual page
a int page_faults = 0;

void init_page_table() {
    for (int i = 0; i < TOTAL_PAGES; i++) {
        page_table[i].frame = -1;  // No frame allocated initially
        page_table[i].last_used = -1;
        page_table[i].valid = 0;
    }

    for (int i = 0; i < TOTAL_FRAMES; i++) {
        reverse_mapping[i] = -1;  // No reverse mapping initially
    }
}

int lru_replace() {
    int oldest_page = -1;
    int oldest_time = INT_MAX;  // INT_MAX is defined in limits.h

    // Find the least recently used page
    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (page_table[i].valid && page_table[i].last_used < oldest_time) {
            oldest_time = page_table[i].last_used;
            oldest_page = i;
        }
    }

    return oldest_page;
}

int translate_virtual_address(uint64_t virtual_address) {
    int virtual_page = (virtual_address >> 12) & 0xFF;  // Extract the virtual page number
    int physical_frame = page_table[virtual_page].frame;

    if (physical_frame == -1) {
        // Page fault: We need to allocate a new frame
        page_faults++;
        physical_frame = allocate_frame();

        if (physical_frame == -1) {
            // No free frames: Use LRU replacement
            int page_to_evict = lru_replace();
            free_frame(page_table[page_to_evict].frame);
            physical_frame = allocate_frame();
        }

        // Update the page table entry
        page_table[virtual_page].frame = physical_frame;
        page_table[virtual_page].valid = 1;
        page_table[virtual_page].last_used = time(NULL);  // Update the timestamp for LRU

        // Update the reverse mapping
        reverse_mapping[physical_frame] = virtual_page;
    } else {
        // Update LRU timestamp
        page_table[virtual_page].last_used = time(NULL);
    }

    return physical_frame;
}
