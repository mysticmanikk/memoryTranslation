CC = gcc
CFLAGS = -Wall -g

# All files to be compiled
SRC = mempart2.c pagetable.c phyframe.c
OBJ = $(SRC:.c=.o)

# Executable name
EXEC = mempart2

# Default target to build the program
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Rule to generate object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(EXEC) part2out p2result.txt
