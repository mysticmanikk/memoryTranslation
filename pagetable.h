// pagetable.h

#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdint.h>

#define TOTAL_PAGES 128  // Number of virtual pages

// Page Table Entry (holds a frame number, last used timestamp, and validity)
typedef struct {
    int frame;          // Physical frame assigned to this virtual page
    int last_used;      // Timestamp of the last access (for LRU policy)
    int valid;          // Validity flag to indicate if the page is loaded in memory
} PageTableEntry;

// Page table and reverse mapping
extern PageTableEntry page_table[TOTAL_PAGES];
extern int reverse_mapping[256];  // Array to map physical frames to virtual pages
extern int page_faults;  // Count of page faults

// Function to initialize the page table and reverse mapping
void init_page_table();

// Function to translate a virtual address to a physical frame (with LRU management)
int translate_virtual_address(uint64_t virtual_address);

// Function to handle LRU page replacement
int lru_replace();

#endif // PAGETABLE_H
