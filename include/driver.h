/*
 * Defines the the driver for the Binary File System (BFS)
 * Built off of the original diskdrv.h, but then divided into two different files
 * driver.h will only read and write to bfs, and nothing else
 * bfs.h will define the structure of the bfs and nothing else
*/

#ifndef _BFS_DRIVER_H
#define _BFS_DRIVER_H

#include "bfs.h"

// input -- read
// returns int, how many chars/bytes read in
// and fills given buffer with data from bfs
int block_read(char* r_data, int blk_no, bfs* device);

// output -- write
// returns int, how many chars/bytes wrote out
int block_write(char* w_data, int blk_no, bfs* device);


#endif