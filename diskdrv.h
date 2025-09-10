#include "buffer.h"
#include <stdio.h>
#ifndef _DISK_DRV_H
#define _DISK_DRV_H

#define VFS "test.vfs"

// should be able to handle disk read and disk writes for 
//      Buffer -> data blocks
//      Inodes themselves -> inode table
//      Write Superblock -> super block
//      

typedef struct diskdrv diskdrv;
struct diskdrv {
    FILE* fs;
    long super_block_start, inode_table_start, data_block_start;
};

diskdrv* ssd;

// Initilaizes a device given path
// sets start of data block
diskdrv* initialize_fs(const char* vfs_path);


// input -- read
// returns int, how many chars/bytes read in
// and fills given buffer with data from vfs
size_t data_block_read(Buffer* r_buf, diskdrv* device);


// output -- write
// returns int, how many chars/bytes wrote out
size_t data_block_write(Buffer* w_buf, diskdrv* device);


#endif