#ifndef PAGETABLE_H
#define PAGETABLE_H

void init_page_table();
int translate_virtual_address(uint64_t virtual_address);

extern int page_faults;

#endif
