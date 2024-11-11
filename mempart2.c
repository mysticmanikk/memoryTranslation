#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "pagetable.h"
#include "phyframe.h"

#define OUTPUT_FILE_NAME "part2out"

// Function to analyze the access sequence and translate virtual addresses to physical addresses
void analyze_access_sequence(const char *input_file) {
    uint64_t virtual_address;
    FILE *content = fopen(input_file, "rb");
    FILE *fout = fopen(OUTPUT_FILE_NAME, "w");

    if (!content || !fout) {
        printf("Error opening file\n");
        return;
    }

    // Read each virtual address from the input file
    while (fread(&virtual_address, sizeof(virtual_address), 1, content) == 1) {
        // Translate the virtual address to the corresponding physical frame
        int physical_frame = translate_virtual_address(virtual_address);

        // Translate the physical address (preserving the page bits)
        uint64_t physical_address = (virtual_address & 0xFFFFFFFFFFFFF000) | physical_frame;

        // Write the translated physical address to the output file in the required format
        fprintf(fout, "0x%016lx\n", physical_address);
    }

    fclose(content);  // Close the input file
    fclose(fout);     // Close the output file
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file_name>\n", argv[0]);
        return 1;
    }

    const char *input_file_name = argv[1];

    // Initialize frames and page table
    init_frames();
    init_page_table();

    // Analyze the access sequence from the input file
    analyze_access_sequence(input_file_name);

    // Output the number of page faults
    printf("Page faults: %d\n", page_faults);

    // Generate and store the MD5 checksum of the output file (part2out)
    system("md5sum part2out > p2result.txt");

    return 0;
}
