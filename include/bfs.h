/*
 * Defines the Binary File System (BFS)
 * Built off of the original diskdrv.h, but then divided into two different files
 * bfs.h will define the structure of the bfs and nothing else
 * driver.h will only read and write to bfs, and nothing else
*/
#ifndef _BINARY_FS_H
#define _BINARY_FS_H

#include "buffer.h"
#include <unistd.h>
#include <fcntl.h>

#include "bfs.h"
#include "util.h"
#include "talloc.h"
#include "su_blk.h"

#define PRE_BLOCK_COUNT 0           // 1 empty block @ beginning
#define SUPER_BLOCK_COUNT 1         // 1 block i.e. 1024 bytes
#define INODE_BLOCK_COUNT 10        // 10 blocks i.e. 10 * 1024 bytes
#define DATA_BLOCK_COUNT 1e4        // 10K Blocks of data blocks. Real size TBD

#define SUBLK_INDEX 0               // used for writing to disk...

// DEFINES the filesize of the filesystem
#define BFS_SIZE (BLOCK_SIZE * (PRE_BLOCK_COUNT + SUPER_BLOCK_COUNT \
                                + INODE_BLOCK_COUNT + DATA_BLOCK_COUNT))

typedef struct bfs bfs;
struct bfs {
    int bfs;
    sublk * incore_sublk;

};

bfs* floppy;

// Initilaizes a device given path
// sets start of data block
bfs* mkbfs(const char * bfs_path);

int setup_free_inode_list(bfs* dev);
int setup_free_block_list(bfs* dev);

//TODO: Functions to be implemented

// Allocates a block from the free list and returns to user. 
// A rough outline can be found on page 86 of UNIX book
Buffer * alloc_buffer(bfs* dev);

// No psuedo-code provided but its roughly the reverse of the alloc function
// Freed block is put into the super_block free_block_list
// if free_block_list is full, then the freed block becomes a new link
//  chain of free blocks, and the content of free_block_list are moved to 
//  free block as an array, and the block is then placed in the free_block_list

int free_buffer(uint32_t block_num);

#endif