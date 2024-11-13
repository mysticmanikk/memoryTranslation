#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "pagetable.h"
#include "phyframe.h"

#define OUTPUT_FILE_NAME "part2out"
#define RESULT_FILE_NAME "p2result.txt"

void analyze_access_sequence(const char *input_file, FILE *fout) {
    uint64_t virtual_address;
    FILE *content = fopen(input_file, "rb");

    if (!content || !fout) {
        printf("Error opening file\n");
        return;
    }

    while (fread(&virtual_address, sizeof(virtual_address), 1, content) == 1) {
        int physical_frame = translate_virtual_address(virtual_address);
        uint64_t physical_address = (physical_frame << 12) | (virtual_address & 0xFFF);
        fprintf(fout, "0x%016lx\n", physical_address);
    }

    fclose(content);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file_name>\n", argv[0]);
        return 1;
    }

    const char *input_file_name = argv[1];

    init_frames();
    init_page_table();

    // Open output file for translated physical addresses
    FILE *fout = fopen(OUTPUT_FILE_NAME, "w");

    if (!fout) {
        printf("Error opening output file\n");
        return 1;
    }

    // Analyze the access sequence and write translated addresses to part2out
    analyze_access_sequence(input_file_name, fout);
    fclose(fout);

    // After processing, write page faults count and MD5 hash to p2result.txt
    FILE *result_file = fopen(RESULT_FILE_NAME, "w");

    if (!result_file) {
        printf("Error opening result file\n");
        return 1;
    }

    // Write page fault count to result file
    fprintf(result_file, "Page faults: %d\n", page_faults);

    // Close the result file before running md5sum command
    fclose(result_file);

    // Generate MD5 hash of part2out and append it to p2result.txt
    system("md5sum part2out >> p2result.txt");

    return 0;
}
