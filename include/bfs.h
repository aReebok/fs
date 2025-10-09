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

#define PRE_BLOCK_SIZE 2 // 2 empty blocks at the beginning
#define SUPER_BLOCK_SIZE 1 // 1 block i.e. 1024 bytes
#define INODE_BLOCK_SIZE 1 // 256 blocks i.e. 10 * 1024 bytes
#define DATA_BLOCK_SIZE 1e4 // 10K Blocks of data blocks. Real size TBD

#define SUBLOCK_NUM -1// used for writing to disk...

// DEFINES the filesize of the filesystem
#define BFS_SIZE (BLOCK_SIZE * (PRE_BLOCK_SIZE + SUPER_BLOCK_SIZE \
                                + INODE_BLOCK_SIZE + DATA_BLOCK_SIZE))

typedef struct bfs bfs;
struct bfs {
    int bfs;
    sublk * incore_sblk;

};

extern bfs * floppy;

// Initilaizes a device given path
// sets start of data block
bfs * mkbfs(const char * bfs_path);

int setup_free_block_list(bfs * dev);

#endif