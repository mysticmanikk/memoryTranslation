#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "pagetable.h"
#include "phyframe.h"

#define OUTPUT_FILE_NAME "part2out"

void analyze_access_sequence(const char *input_file) {
    uint64_t virtual_address;
    FILE *content = fopen(input_file, "rb");
    FILE *fout = fopen(OUTPUT_FILE_NAME, "w");

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
    fclose(fout);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file_name>\n", argv[0]);
        return 1;
    }

    const char *input_file_name = argv[1];

    init_frames();
    init_page_table();

    analyze_access_sequence(input_file_name);

    printf("Page faults: %d\n", page_faults);
    system("md5sum part2out > p2result.txt");

    return 0;
}
