README 
# memoryTranslation

Maksym Yarosh 2881321 50%
Manikdeep Kaur Lnu 2855864 50%

Project Overview
This project simulates memory address translation for a process in a simple operating system with a 1KB physical memory and 12-bit virtual addresses. The simulator handles virtual-to-physical address translation using a page table and employs memory management techniques such as page fault handling and Least Recently Used (LRU) page replacement.

The project is divided into two parts:

Part 1: Basic address translation (virtual addresse to physical address) using a static page table.
Part 2: Dynamic page table management with LRU page replacement policy and reverse mapping.

Project Files
Part 1 Files:
mempart1.c: Translates virtual addresses to physical addresses using a static page table.
Part 2 Files:
phyframe.c: Manages physical frames, allocation, and replacement.
pagetable.c: Handles page table management, LRU page replacment policy and reverse mapping.
mempart2.c: Main program for Part 2, which processes the input and manages page faults and address translation. It processes the automated production of p2result.txt file.
phyframe.h: function prototype for managing frame: inititlaizing, allocating and freeing up frames.
pagetable.h: contains function prototype for initilaizing page table and managing the translate virtual address.
Input and Output:
Input Files: Binary files containing sequences of virtual addresses (part1test, part1sequence, and part2sequence).
Output Files:
Part 1: part1out - Physical addresses for Part 1 and p1result.txt - Contains the MD5 checksum manually run on part1out file.
Part 2: part2out - Physical addresses for Part 2 and p2result.txt - Contains the MD5 checksum and page fault count (automated within the mempart2.c; no need to generate the md5sum hash value for the part2out manually). The p2result.txt is generated when you run the command ./mempart2 part2sequence

Part1: 

In Part 1, mempart1.c performs address translation using a predefined static page table with the page-to-frame mappings. This file reads virtual addresses, translates them based on the page table, and writes the physical addresses to part1out.

Execution
Compile and run mempart1.c:

gcc mempart1.c -o mempart1
./mempart1 part1test


The output file part1out will contain the physical addresses in hexadecimal format for some virtual addresses. The sample output for this will look as follows:
0x0144
0x01A4
0x00E8
0x028C
0x025C
0x01B4
0x0218
0x0250

After verifying correctness with part1test, use part1sequence as the input to produce the final output. The md5sum utility is run on part1out and the hash value is written to p1result.txt as follows:

./mempart1 part1sequence
md5sum part1out > p1result.txt


Part 2: Dynamic Address Translation with LRU

In Part 2, address translation is dynamic. We simulate page fault handling with a page replacement policy using an LRU approach. The physical frames are managed using a reverse mapping and an LRU counter mechanism.

Module Functions:
phyframe.c:
initialize_physical_frames: Initializes frame management, with frame 0 reserved for OS.
allocate_frame: Allocates frames sequentially and replaces the LRU frame when no frames are free.
free_frame: free the frame when the allocated frames are full. 

pagetable.c:
initialize_page_table: Initializes page table entries as invalid.
translate_address: Translates virtual addresses, handling page faults and updating LRU information.
lru_replace: Identifies the least recently used frame for replacement.

Compile and run Part 2:

make
./mempart2 part2sequence


The output part2out will contain the physical addresses, and p2result.txt will show the MD5 checksum and page fault count. There is no need to run the md5sum command manually as checksum is run as an in-built function withint he program.

Conclusion

This project demonstrates memory address translation and management in a virtual memory environment, simulating the process-level page management and hardware-level address translation. The program dynamically allocates pages, handles page faults, and uses LRU replacement to manage memory effectively.

This README document outlines each step required to execute the project, provides troubleshooting guidance, and includes sample outputs.
