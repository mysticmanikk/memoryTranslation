// pagetable.c
#include "memory.h"
#include <stdio.h>

void initialize_page_table(PageTable *pt) {
    for (int i = 0; i < NUM_PAGES; i++) {
        pt->entries[i].frame_number = -1; // -1 indicates no mapping
        pt->entries[i].valid = 0;
    }
}

unsigned long translate_address(PageTable *pt, PhysicalFrames *pf, unsigned long virtual_address, int *page_fault_count, int access_time) {
    unsigned int page_num = (virtual_address >> 7) & 0x0F;
    unsigned int offset = virtual_address & 0x7F;

    if (page_num >= NUM_PAGES || !pt->entries[page_num].valid) {
        (*page_fault_count)++;
        int frame_num = allocate_frame(pf, page_num);

        // If frame already has a page, mark it invalid in the page table
        if (pf->frame_to_page[frame_num] != -1) {
            int old_page = pf->frame_to_page[frame_num];
            pt->entries[old_page].valid = 0;
        }

        // Map page to frame and update reverse mapping
        pt->entries[page_num].frame_number = frame_num;
        pt->entries[page_num].valid = 1;
        pf->frame_to_page[frame_num] = page_num;
    }

    int frame_num = pt->entries[page_num].frame_number;
    update_page_usage(pf, frame_num, access_time);

    return (frame_num << 7) | offset;
}
