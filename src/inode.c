#include <stdio.h>
#include <stdint.h>
#include "inode.h"
#include "talloc.h"

DiskInode* create_dinode(uint8_t fileown, 
    uint8_t filtyp, 
    time_t lastacc,
    time_t lastmod,
    time_t inolastmod,
    uint8_t links,
    size_t fsize
) {
    DiskInode* temp_dino = (DiskInode*) talloc(sizeof(DiskInode));
    temp_dino->file_owner_identifier = fileown;
    temp_dino->file_type = filtyp;
    temp_dino->last_accessed = lastacc;
    temp_dino->last_modified = lastmod;
    temp_dino->inode_last_modified = inolastmod;
    temp_dino->links = links;
    // temp_dino->table_of_contents = toc;
    temp_dino->file_size = fsize;

    return temp_dino;
}

Inode* create_inode(DiskInode *dino, 
    const int status, 
    const int dev_no, 
    const int inum
) {
    Inode* temp_ino = (Inode*) talloc(sizeof(Inode));
    temp_ino->dino = dino;
    temp_ino->status = status;
    temp_ino->logical_device_no = dev_no;
    temp_ino->inode_number = inum;
    temp_ino->reference_count = 0;

    return temp_ino;
}

int hash_inode(const Inode *const ino) {
    return ino->inode_number % IHASH_SIZE;
}
int hash_inonum(int inonum) {
    return inonum % IHASH_SIZE;
}
