#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "diskdrv.h"
#include "talloc.h"

#define VFS "test.vfs"

diskdrv device;


// Helper functions ----
void drv_showpos(FILE* fp) {
    long pos = ftell(fp);
    printf("file position: %ld\n", pos);
} 

int device_init() {
    device.fs = fopen(VFS, "rb+");
    if (!device.fs) {
        perror("File system opening failed");
        printf( "file %s::%d\n", __FILE__, __LINE__);
        texit(EXIT_FAILURE);
    }
    
    char buf[100]; 
    int counter = 0;
    char c;
    while ((c = fgetc(device.fs)) != EOF && (c != '\n')) buf[counter++] = c;
    buf[counter] = '\0';
    if (strcmp(buf, "/* disk block */") == 0) device.data_block_start = ftell(device.fs);
    return 0;
}

// given a file pointer -> jump to the beginning of line 
// returns the index of the block found from the start position
// lines -> block no?
int jump(FILE* fp, long start_position, int block_no) {
    if (fseek(fp, start_position, 0) != 0) {
        perror("fseek failed"); // write better errors
        texit(1);
    }

    int block_found = 0;
    char c;
    while (block_found < block_no && (c = fgetc(fp)) != EOF )
        if (c == '\n') block_found++;           
    return block_found;
}

size_t read_curr_block(FILE* fp, char* data) {
    char c;
    int i = 0;
    while (i < BLOCK_SIZE - 1 && (c = fgetc(fp)) != EOF && c != '\n')
        data[i++] = c;
    data[i] = '\0'; 
    return i;
}

// Implementation -----

size_t data_block_read(Buffer* r_buf) {
    int len = jump(device.fs, device.data_block_start, r_buf->block_no);
    return read_curr_block(device.fs, r_buf->data);
}

size_t data_block_write(Buffer* w_buf){
    int len = jump(device.fs, device.data_block_start, w_buf->block_no);
    fseek(device.fs, ftell(device.fs), SEEK_SET);
    return fwrite(w_buf->data, sizeof(char), 5, device.fs);
}

int main( ) {
    device_init();

    Buffer* buf = create_buf(1, 4, 0);
    data_block_read(buf);
    printf("Buffer content:%s\n" , buf->data);

    // write to disk
    buf->data[4] = 'P';
    int write_l = data_block_write(buf);
    // printf(">> ftell location after write %ld\n", ftell(device.fs));
    // printf(">> ftell location of disk block start %ld\n", device.data_block_start);
    // printf("write_l: %d\n", write_l);
    fclose(device.fs);
    return 0;
}