README 
# memoryTranslation

Maksym Yarosh 2881321 50%
Manikdeep Kaur Lnu 2855864 50%

Project Overview
This project simulates memory address translation for a process in a simple operating system with a 1KB physical memory and 12-bit virtual addresses. The simulator handles virtual-to-physical address translation using a page table and employs memory management techniques such as page fault handling and Least Recently Used (LRU) page replacement.

The project is divided into two parts:

Part 1: Basic address translation using a static page table.
Part 2: Dynamic page table management with LRU page replacement policy.

Project Files
Part 1 Files:
mempart1.c: Translates virtual addresses to physical addresses using a static page table.
Part 2 Files:
phyframe.c: Manages physical frames, allocation, and replacement.
pagetable.c: Handles page table management and reverse mapping.
mempart2.c: Main program for Part 2, which processes the input and manages page faults and address translation.
memory.h: Header file defining structures and constants shared by the other files.
Input and Output:
Input Files: Binary files containing sequences of virtual addresses (part1test, part1sequence, and part2sequence).
Output Files:
Part 1: part1out - Physical addresses for Part 1.
Part 2: part2out - Physical addresses for Part 2 and p2result.txt - Contains the MD5 checksum and page fault count.

Part1: 

n Part 1, mempart1.c performs address translation using a predefined static page table with the page-to-frame mappings. This file reads virtual addresses, translates them based on the page table, and writes the physical addresses to part1out.

Execution
Compile and run mempart1.c:

gcc mempart1.c -o mempart1
./mempart1 part1test

The output file part1out will contain the physical addresses in hexadecimal format. After verifying correctness with part1test, use part1sequence as the input to produce the final output.

Part 2: Dynamic Address Translation with LRU

In Part 2, address translation is dynamic. We simulate page fault handling with a page replacement policy using an LRU approach. The physical frames are managed using a reverse mapping and an LRU counter mechanism.

Module Functions:
phyframe.c:
initialize_physical_frames: Initializes frame management, with frame 0 reserved for OS.
allocate_frame: Allocates frames sequentially and replaces the LRU frame when no frames are free.
update_page_usage: Updates the LRU counter.
lru_replace: Identifies the least recently used frame for replacement.
pagetable.c:
initialize_page_table: Initializes page table entries as invalid.
translate_address: Translates virtual addresses, handling page faults and updating LRU information.

Compile and run Part 2:

gcc mempart2.c phyframe.c pagetable.c -o mempart2
./mempart2 part2sequence

The output part2out will contain the physical addresses, and p2result.txt will show the MD5 checksum and page fault count.

Conclusion

This project demonstrates memory address translation and management in a virtual memory environment, simulating the process-level page management and hardware-level address translation. The program dynamically allocates pages, handles page faults, and uses LRU replacement to manage memory effectively.

This README document outlines each step required to execute the project, provides troubleshooting guidance, and includes sample outputs.
