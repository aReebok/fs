#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "diskdrv.h"
#include "talloc.h"

diskdrv * ssd;

// Helper functions ----
void drv_showpos(FILE* fp) {
    long pos = ftell(fp);
    printf("file position: %ld\n", pos);
} 

int fseek_line(FILE *fp, int line_no) {
    rewind(fp);
    char buf[BLOCK_SIZE];
    for (int i = 1; i < line_no; i++) {
        if (!fgets(buf, sizeof(buf), fp)) {
            return -1; // EOF or error
        }
    }
    return 0;
}
diskdrv* initialize_fs(const char* vfs_path) {
    diskdrv* device = talloc(sizeof(diskdrv));
    device->fs = fopen(vfs_path, "rb+");
    if (!device->fs) {
        perror("File system opening failed");
        printf("file %s::%d\n", __FILE__, __LINE__);
        texit(EXIT_FAILURE);
    }

    char buf[BLOCK_SIZE];
    char section[64];
    int line_no;

    int super_line = -1, inode_line = -1, data_line = -1;

    int current_line = 1;
    rewind(device->fs);

    while (fgets(buf, sizeof(buf), device->fs)) {
        // Beginning of this line
        long line_start = ftell(device->fs) - strlen(buf);

        // --- Check if this is a header line ---
        if (current_line <= 10 && sscanf(buf, "%63s %d", section, &line_no) == 2) {
            if (strcmp(section, "super_block") == 0) super_line = line_no;
            else if (strcmp(section, "inode_table") == 0) inode_line = line_no;
            else if (strcmp(section, "disk_block") == 0) data_line = line_no;
        }

        // --- Check if this line matches any section start ---
        if (current_line == super_line) device->super_block_start = line_start;
        if (current_line == inode_line) device->inode_table_start = line_start;
        if (current_line == data_line) device->data_block_start = line_start;

        current_line++;
    }

    return device;
}

// given a file pointer -> jump to the beginning of line 
// returns the index of the block found from the start position
// lines -> block no?
int jump(FILE* fp, long start_position, int block_no) {
    if (fseek(fp, start_position, SEEK_SET) != 0) {
        perror("fseek failed"); // write better errors
        texit(1);
    }

    int block_read = 0;
    char c[BLOCK_SIZE];
    while (block_read < block_no && fgets(c, sizeof(c), fp)) 
        block_read++;  
    return block_read;
}

size_t read_curr_block(FILE* fp, char* data) {
    // TODO: combine fgets + strlen, 
    if (!fgets(data, BLOCK_SIZE, fp)) 
        return 0;
    int len = strlen(data);
    data[len-1] = '\0';
    return len; 
}

// Implementation -----

size_t data_block_read(Buffer* r_buf, diskdrv* device) {
    int index = jump(device->fs, device->data_block_start, r_buf->block_no);
    if (index != r_buf->block_no) {  // check to see if reached correct block 
        printf("jumped to an incorrect block number while reading file: find[%d]: actual ret[%d]\n", r_buf->block_no, index);
        texit(1);
    }
    return read_curr_block(device->fs, r_buf->data);
}

size_t data_block_write(Buffer* w_buf, diskdrv* device){

    int index = jump(device->fs, device->data_block_start, w_buf->block_no);
    printf("Index: %d\n", index);
    if (index != w_buf->block_no) {  // check to see if reached correct block 
        printf("jumped to an incorrect block number while reading file: find[%d]: actual ret[%d]\n", w_buf->block_no, index);
        texit(1);
    }
    fseek(device->fs, ftell(device->fs), SEEK_SET);
    return fwrite(w_buf->data, sizeof(char), strlen(w_buf->data), device->fs); // TODO optimize by removing strlen
}