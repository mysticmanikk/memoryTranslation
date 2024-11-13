#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "phyframe.h"

#define PAGES 32    // virtual pages
#define FRAMES 8    // physical frames

typedef struct Node {
    int page;                // Virtual page number
    int frame;               // Physical frame number
    struct Node *prev, *next;
} Node;

typedef struct {
    Node *head, *tail;
} LRUList;

typedef struct {
    int frame;               // Physical frame mapped to this virtual page
    int valid;               // Valid bit indicating if page is in physical memory
} PageTableEntry;

PageTableEntry page_table[PAGES];  // Page table
Node *frame_to_page_map[FRAMES];   // Reverse mapping from frame -> page (hash table)

LRUList lru_list = { NULL, NULL }; // LRU list
int page_faults = 0;

// Function prototypes
void add_to_front(Node *node);
void move_to_front(Node *node);
int evict_lru_page(int new_page, uint64_t virtual_address);
Node* create_node(int page, int frame);

// Initialize page table and reverse mappings
void init_page_table() {
    for (int i = 0; i < PAGES; i++) {
        page_table[i].frame = -1;
        page_table[i].valid = 0;
    }
    for (int i = 0; i < FRAMES; i++) {
        frame_to_page_map[i] = NULL;
    }
}

// Add a node to the front of the LRU list
void add_to_front(Node *node) {
    node->next = lru_list.head;
    node->prev = NULL;
    if (lru_list.head) lru_list.head->prev = node;
    lru_list.head = node;
    if (!lru_list.tail) lru_list.tail = node;
}

// Move a node to the front of the LRU list
void move_to_front(Node *node) {
    if (node == lru_list.head) return;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (node == lru_list.tail) lru_list.tail = node->prev;
    add_to_front(node);
}

// Evict the least recently used page (tail of the LRU list)
int evict_lru_page(int new_page, uint64_t virtual_address) {
    Node *lru_node = lru_list.tail;
    if (!lru_node) return -1;

    int evicted_page = lru_node->page;
    int frame = lru_node->frame;

    // Mark evicted page as invalid and clear reverse mapping
    page_table[evicted_page].valid = 0;
    frame_to_page_map[frame] = NULL;

    printf("Evicting page %d (virtual address) from frame %d; now mapping page %d to this frame\n",
           evicted_page, frame, new_page);

    // Remove LRU node from list
    if (lru_node->prev) lru_node->prev->next = NULL;
    lru_list.tail = lru_node->prev;
    if (!lru_list.tail) lru_list.head = NULL;

    free(lru_node);
    return frame;  // Return the freed frame
}

// Create a new node for the LRU list
Node* create_node(int page, int frame) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->page = page;
    node->frame = frame;
    node->prev = node->next = NULL;
    return node;
}

// Translate a virtual address to a physical address, handle LRU and page faults
int translate_virtual_address(uint64_t virtual_address) {
    int virtual_page = (virtual_address >> 7) & 0x7F; // Extract virtual page number
    int physical_frame = page_table[virtual_page].frame;

    if (!page_table[virtual_page].valid) {
        // Page fault occurs
        page_faults++;

        physical_frame = allocate_frame();
        if (physical_frame == -1) {
            // No free frame available; evict LRU page and reuse the frame
            physical_frame = evict_lru_page(virtual_page, virtual_address);
            if (physical_frame == -1) {
                printf("Error: Failed to evict page, no frame available.\n");
                exit(1);
            }
        }

        // Map the page to the allocated frame
        page_table[virtual_page].frame = physical_frame;
        page_table[virtual_page].valid = 1;

        // Create a new node for the LRU list and update reverse mapping
        Node *new_node = create_node(virtual_page, physical_frame);
        frame_to_page_map[physical_frame] = new_node;
        add_to_front(new_node);

        printf("Page %d with virtual address 0x%016lx mapped to frame %d\n",
               virtual_page, virtual_address, physical_frame);
    } else {
        // Page hit: move node to front of LRU list
        Node *node = frame_to_page_map[physical_frame];
        if (node) move_to_front(node);
    }

    return physical_frame;
}
