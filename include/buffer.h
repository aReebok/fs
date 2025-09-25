#ifndef _BUFFER_H
#define _BUFFER_H

#include "cdllist.h"

#define BLOCK_SIZE  1024        // File System block size 
#define HASH_SIZE   4

#define B_LOCKED    0x01        // Buffer Status Flags
#define B_VALID     0x02
#define B_DELWRI    0x04
#define B_IOBUSY    0x08
#define B_WANTED    0x16

// TODO: Create inline functins to set Buffer status 

typedef struct Buffer Buffer;

struct Buffer { 
    unsigned int device_no;
    unsigned int block_no;
    unsigned int status;
    char data[BLOCK_SIZE];
    cdllist fl_hook;
    cdllist hq_hook; 
};

// Create a new buffer object with given device, block, and status flags.
Buffer *create_buf(const int devno,
                   const int blockno,
                   const int status);

// Compute the hash queue index for a buffer (0 .. HASH_SIZE-1).
int hash_buffer(const Buffer *const buf);
int hash_block_num(int const block_no);

// Overwrite the status flags of a buffer.
void set_buf_status(Buffer *const buf, unsigned int status);
void print_buffer(Buffer * const buf);

#endif // _BUFFER_H