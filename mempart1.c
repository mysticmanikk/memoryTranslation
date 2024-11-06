#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 128   // Size of each page/frame in bytes
#define NUM_PAGES 11    // Number of page table entries
#define NUM_FRAMES 8    // Number of physical frames

// Hard-coded page table mapping for Part 1 (virtual page -> physical frame)
int page_table[NUM_PAGES] = {2, 4, 1, 7, 3, 5, 6, -1, -1, -1, -1};

// Function to translate virtual address to physical address
unsigned long translate_address(unsigned long virtual_address) {
    unsigned int page_number = (virtual_address >> 7) & 0x0F; // Top 4 bits for page number (12-bit address)
    unsigned int offset = virtual_address & 0x7F;             // Bottom 7 bits for offset (128-byte pages)

    // Check if the page number is mapped
    if (page_number >= NUM_PAGES || page_table[page_number] == -1) {
        fprintf(stderr, "Invalid page access for page number %u\n", page_number);
        exit(1); // Exit if the page is not mapped
    }

    // Get the frame number from the page table and compute the physical address
    int frame_number = page_table[page_number];
    unsigned long physical_address = (frame_number << 7) | offset;

    return physical_address;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input filename>\n", argv[0]);
        return 1;
    }

    // Open the input file containing virtual addresses
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing physical addresses
    FILE *output_file = fopen("part1out", "w");
    if (output_file == NULL) {
        perror("Error creating output file");
        fclose(input_file);
        return 1;
    }

    unsigned long virtual_address;
    while (fread(&virtual_address, sizeof(unsigned long), 1, input_file) == 1) {
        // Translate virtual address to physical address
        unsigned long physical_address = translate_address(virtual_address);

        // Write the physical address to output file in hexadecimal format with "0x" prefix
        fprintf(output_file, "0x%04lX\n", physical_address);
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    return 0;
}
