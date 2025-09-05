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

typedef struct Buffer Buffer;

struct Buffer { 
    unsigned int device_no;
    unsigned int block_no;
    unsigned int status;
    char data[BLOCK_SIZE]; // for now just chars
    // hash queue list TO DO
    cdllist fl_hook; // embedded, contains a prev: tail and next: head
    cdllist hq_hook; 
};

// Create a new buffer object with given device, block, and status flags.
Buffer *create_buf(const unsigned int devno,
                   const unsigned int blockno,
                   const unsigned int status);

// Compute the hash queue index for a buffer (0 .. HASH_SIZE-1).
int hash_buffer(const Buffer *const buf);

// Overwrite the status flags of a buffer.
void set_buf_status(Buffer *const buf, unsigned int status);

#endif // _BUFFER_H