#ifndef _INODE_H
#define _INODE_H
/**
 * In-core inode
 */
// #include <ctime>
#include "cdllist.h"
#include <time.h>
#include <stdint.h>

#define IHASH_SIZE  4

#define I_LOCKED    0x01        // inode locked
#define I_INODIF    0x02        // in-core inode modified
#define I_FILDIF    0x04        // file content itself modified
#define I_MNTPT     0x08        // is file a mount point
#define I_WANTED    0x16        // another proc is waiting for this inode

typedef struct DiskInode DiskInode;
struct DiskInode { // 40 bytes
    // on-disk 
    uint8_t file_owner_identifier; // {Owner: areeba, group: dev}
    uint8_t file_type; // regular, directory, charachter special, block special, pipes (FIFOs).
    uint8_t links; // number of links to the file; number of names a file has
    uint16_t file_size;
    uint32_t table_of_contents[11]; // i think it's the 8 direct/3 indirect block array stuff
    uint32_t last_accessed, last_modified, inode_last_modified;
};

typedef struct Inode Inode; // 94 bytes
struct Inode {
    DiskInode *dino;

    int status; 
                                // information to find inode on disk
    int logical_device_no;      // devno of fs that contains the file
    int inode_number;           // [incore-only] disk inode are stored in an array, this is the index on that arr

    cdllist fl_hook;            // inode hash queue
    cdllist hq_hook;            // inode free list 
    int reference_count; // inode reference count to active instances of this file
};

DiskInode* create_dinode(uint8_t fileown, 
    uint8_t filtyp, 
    time_t lastacc,
    time_t lastmod,
    time_t inolastmod,
    uint8_t links,
    size_t fsize
);

Inode* create_inode(DiskInode* dino, 
    const int status, 
    const int dev_no, 
    const int inum
);

int hash_inode(const Inode *const ino);
int hash_inonum(int inonum);


#endif // _INODE_H