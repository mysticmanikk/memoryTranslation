#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "phyframe.h"  // Include the header for frame management

#define PAGES 128  // virtual pages
#define FRAMES 256  // physical frames

typedef struct {
    int frame;  // Physical frame mapped to this virtual page
    int last_used;  // Timestamp for LRU
    int valid;  // Flag to indicate if the page is valid
} PageTableEntry;

PageTableEntry page_table[PAGES];  // Page table for the virtual pages
int reverse_mapping[FRAMES];  // Reverse mapping from frame to virtual page
int page_faults = 0;

void init_page_table() {
    // Initialize the page table
    for (int i = 0; i < PAGES; i++) {
        page_table[i].frame = -1;  // No frame allocated initially
        page_table[i].last_used = -1;
        page_table[i].valid = 0;
    }

    // Initialize reverse mapping (no virtual page mapped to frame initially)
    for (int i = 0; i < FRAMES; i++) {
        reverse_mapping[i] = -1;
    }
}

int lru_replace() {
    // Implement LRU policy: find the least recently used page
    int oldest_page = -1;
    int oldest_time = INT_MAX;  // Set to a large number initially

    for (int i = 0; i < PAGES; i++) {
        if (page_table[i].valid && page_table[i].last_used < oldest_time) {
            oldest_time = page_table[i].last_used;
            oldest_page = i;
        }
    }

    return oldest_page;  // Return the page index of the least recently used page
}

int translate_virtual_address(uint64_t virtual_address) {
    // Extract the virtual page from the virtual address
    int virtual_page = (virtual_address >> 7) & 0x7F;  // Extract the virtual page number (128-byte pages)
    int physical_frame = page_table[virtual_page].frame;

    if (physical_frame == -1) {
        // Page fault: No physical frame allocated for this virtual page
        page_faults++;
        printf("Page fault for virtual address: 0x%016lx\n", virtual_address);  // Debug statement

        physical_frame = allocate_frame();

        if (physical_frame == -1) {
            // No free frames available: Evict a page using the LRU policy
            int page_to_evict = lru_replace();
            printf("Evicting page %d (frame %d)\n", page_to_evict, page_table[page_to_evict].frame);  // Debug statement
            free_frame(page_table[page_to_evict].frame);  // Evict the LRU page
            physical_frame = allocate_frame();
        }

        // Update the page table entry and reverse mapping
        page_table[virtual_page].frame = physical_frame;
        page_table[virtual_page].valid = 1;  // Mark the page as valid
        page_table[virtual_page].last_used = time(NULL);  // Set the timestamp for LRU

        reverse_mapping[physical_frame] = virtual_page;

        printf("Mapped virtual address 0x%016lx to physical frame %d\n", virtual_address, physical_frame);  // Debug statement
    } else {
        // Page hit: Update the LRU timestamp
        page_table[virtual_page].last_used = time(NULL);
    }

    return physical_frame;  // Return the physical frame number
}
