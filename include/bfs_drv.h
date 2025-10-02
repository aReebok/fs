/*
 * Initially (and essentially) a copy of diskdrv.h
 * but will work with binary file instead of text file...
 * Will keep both versions for now, for testing and debugging purposes.
 */
#include "buffer.h"
#include <stdio.h>
#include <string.h>

#ifndef _BINARY_FS_DRIVER_H
#define _BINARY_FS_DRIVER_H

// should be able to handle disk read and disk writes for 
//      Buffer -> data blocks
//      Inodes themselves -> inode table
//      Write Superblock -> super block

#define PRE_BLOCK_SIZE 2 // 2 empty blocks at the beginning
#define SUPER_BLOCK_SIZE 1 // 1 block i.e. 512 bytes
#define INODE_BLOCK_SIZE 256 // 10 blocks i.e. 512 * 10 bytes
#define DATA_BLOCK_SIZE 1e6 // 10K Blocks of data blocks. Real size TBD
#define SUBLOCK_NUM -1// used for writing to disk...

// DEFINES the filesize of the filesystem
#define BFS_SIZE (BLOCK_SIZE * (PRE_BLOCK_SIZE + SUPER_BLOCK_SIZE \
                                + INODE_BLOCK_SIZE + DATA_BLOCK_SIZE))


typedef struct bfs bfs;
struct bfs {
    int bfs;
    long super_block_start, inode_table_start, data_block_start;
};

extern bfs * floppy;

// Initilaizes a device given path
// sets start of data block
bfs* initialize_bfs(const char* bfs_path);

// input -- read
// returns int, how many chars/bytes read in
// and fills given buffer with data from bfs
// size_t data_block_read(Buffer* r_buf, bfs* device);

// output -- write
// returns int, how many chars/bytes wrote out
size_t block_write(Buffer* w_buf, int blk_no, bfs* device);

#endif