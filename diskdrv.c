#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diskdrv.h"
#include "talloc.h"

#define VFS "test.vfs"

diskdrv device;

void drv_showpos(FILE* fp) {
    long pos = ftell(fp);
    printf("file position: %ld\n", pos);
} 

int device_init () {
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
int jump(FILE* fp, int block_no, long start_position) {
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
    while (i < 1023 && (c = fgetc(fp)) != EOF && c != '\n')
        data[i++] = c;
    data[i] = '\0'; 
    return i;
}

size_t data_block_read(char* buf, int block_location) {
    int len = jump(device.fs, block_location, device.data_block_start);
    return read_curr_block(device.fs, buf);
}

size_t data_block_write(char* buf, int block_location){
    int len = jump(device.fs, block_location, device.data_block_start);
    printf("len: %d | wriiting this: %s\n", len, buf);
    printf(">> ftell location before write %ld\n", ftell(device.fs));
    fseek(device.fs, ftell(device.fs), SEEK_SET);
    return fwrite(buf, sizeof(char), 5, device.fs);
}

int main( ) {
    device_init();

    int block_location = 4;

    // read from disk block loc = 4
    char buf[1024];
    data_block_read(buf, block_location);
    printf("Buffer content:%s\n" , buf);

    // write to disk
    buf[4] = 'P';
    int write_l = data_block_write(buf, block_location);
    printf(">> ftell location after write %ld\n", ftell(device.fs));
    printf(">> ftell location of disk block start %ld\n", device.data_block_start);
    printf("write_l: %d\n", write_l);
    fclose(device.fs);
    return 0;
}