// memory.h
#ifndef MEMORY_H
#define MEMORY_H

#define PAGE_SIZE 128
#define NUM_PAGES 11
#define NUM_FRAMES 8

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

typedef struct {
    PageTableEntry entries[NUM_PAGES];
} PageTable;

typedef struct {
    int frame_to_page[NUM_FRAMES];
    int last_used[NUM_FRAMES];   // For LRU tracking
    int next_free_frame;
} PhysicalFrames;

void initialize_page_table(PageTable *pt);
void initialize_physical_frames(PhysicalFrames *pf);
int allocate_frame(PhysicalFrames *pf, int page_num);
void update_page_usage(PhysicalFrames *pf, int frame_num, int access_time);
int lru_replace(PhysicalFrames *pf);
unsigned long translate_address(PageTable *pt, PhysicalFrames *pf, unsigned long virtual_address, int *page_fault_count, int access_time);

#endif
