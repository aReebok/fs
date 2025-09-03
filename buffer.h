#ifndef _BUFFER_H
#define _BUFFER_H

#include "free_list.h"

#define BLOCK_SIZE  1024        // File System block size 
#define BUF_FL_SZ   10
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
    free_list fl_hook; // embedded, contains a prev: tail and next: head
    free_list hq_hook; 
};

// member functions just for buffer struct
Buffer* create_buf(const unsigned int devno, \
    const unsigned int blockno, \
    const unsigned int status);

int hash_buffer(const Buffer* const buf);

void set_buf_status(Buffer* const buf, unsigned int status);

#endif