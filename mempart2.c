// mempart2.c
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input filename>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen("part2out", "w");
    if (output_file == NULL) {
        perror("Error creating output file");
        fclose(input_file);
        return 1;
    }

    PageTable pt;
    PhysicalFrames pf;
    initialize_page_table(&pt);
    initialize_physical_frames(&pf);

    unsigned long virtual_address;
    int page_fault_count = 0, access_time = 0;

    while (fread(&virtual_address, sizeof(unsigned long), 1, input_file) == 1) {
        unsigned long physical_address = translate_address(&pt, &pf, virtual_address, &page_fault_count, access_time);
        fprintf(output_file, "0x%04lX\n", physical_address);
        access_time++;
    }

    fclose(input_file);
    fclose(output_file);

    FILE *result_file = fopen("p2result.txt", "w");
    fprintf(result_file, "Page Faults: %d\n", page_fault_count);
    fclose(result_file);

    return 0;
}
