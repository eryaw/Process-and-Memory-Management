##Adapted from Lab2 COMP30023 Computer System 2020
CC = gcc

CFLAGS = -Wall -Wextra -std=gnu99 -I. #-g -fsanitize=address

OBJ = main.o parseArgu.o processInfo.o queue.o simulateSched.o utilities.o \
	  memoryAlloc.o cpuInfo.o
EXE = scheduler

## Create .o files from .c files. Searches for .c files with same .o names given in OBJ
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

## Create executable linked file from object files. 
$(EXE): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
	
## Run "$ make clean" to remove the object and executable files
clean:
	rm -f $(OBJ) $(EXE)
