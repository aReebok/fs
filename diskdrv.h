#ifndef _DISK_DRV_H
#define _DISK_DRV_H

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

// input -- read
// returns int, how many chars/bytes read in
// and fills given buffer with data from vfs
size_t data_block_read(char* r_buf, int block_location);


// output -- write
// returns int, how many chars/bytes wrote out
size_t data_block_write(char* w_buf, int block_location);

#endif