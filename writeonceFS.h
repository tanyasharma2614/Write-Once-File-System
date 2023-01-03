// File:	writeonceFS.h

// List all group members' names:
// Sanchay Kanade (sk2656)
// Tanya Sharma (tds104)
// iLab machine tested on:

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>

#define CAPACITY 4*1024*1024

#define MIN(x,y) (((x)<(y)) ? (x):(y))

//Charcater pointer representing our memory location where we will mount our file system size of 4MB
char *memory_rep;
//char memory_rep[CAPACITY];

typedef enum {WO_RDONLY, WO_WRONLY, WO_RDWR} flags;

//typedef enum {WO_CREAT, WO_NOCREAT} mode;

//Linked list representation of data blocks
typedef struct dataBlock{
    //unsigned int address_index;
    
    //Stores data
    char data[1020];
    //This will store next datablock index of the file
    int next;
    

} datablock;

typedef struct superblock{
    int numberOfinodesFilled;
    int numberOfblocksFilled;
    //int blockSize;
} sb;

//Current inode size 28 bytes
typedef struct inode{
    
    //Id for each i node
    int id;
    //linked * head;
    //Storing starting index of the file
    unsigned int start_index;

    //How many data blocks in this file
    int numberOfDataBlocks;

    //name of the file
    char name[16];

    //file open flag
    int isOpen;

    //This flag tells in which moe file is opened
    int rwFlag;

    //tells how many bytes are filled in the last data block of a file
    int bytesFilled;

}inode;

//Functions

//This functions creates our disk file of 4MB for operation
void createFS();

int checkDisk(void* memoryAddress);

int wo_mount(char* fileName, void* memoryAddress);

int wo_unmount(void* memoryAddress);

//This creats file if it does not exist. If it exists then returns error. Also opens it 
int wo_create(char* fileName, flags f);

//This function opens an alreadye exixting file. If file does not exists it returns error
int wo_open(char* fileName, flags f);

/*
fd: a valid filedscriptor for your filesystem
buffer: a memory location to either read bytes in to or from
bytes: the number of bytes to read or write*/

int wo_read(int fileDescriptor, void* buffer, int Bytes);

int wo_write(int fileDescriptor, void* buffer, int Bytes);

int wo_close(int fileDescriptor);

//Test functions

//This functions creates 50 files
void saturatingInodes();

//This function writes maximum amount of data possible in a single file
void maximization();