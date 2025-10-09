#include "bfs.h"

#define ERROR() \
    perr("Talloc failed: BFS_DEVICE could not be created");\
    return NULL;

bfs * floppy;

bfs * mkbfs(const char * bfs_path) {
    bfs * dev = talloc(sizeof(*dev));

    if(dev == NULL) {
        ERROR(); // TODO: Test in a safe environment whether the semi-colon or braces are necessary
    }

    int f = open(bfs_path, O_RDWR | O_CREAT | O_TRUNC, 0644); // #TODO: ADD checks to maybe not recreate the file everytime?

    // ftruncate takes the created file and shortens/lengthens it to size of BFS... 0 padded
    if (ftruncate(f, BFS_SIZE) == -1) {
        ERROR();
        printf("teste");
    }

    dev->bfs = f;

    // TODO: Setup super block
    
    // TODO: Setup inode lists and stuff
    // TODO: Setup free block list


    // printf("======Successfully created BFS of size: %d bytes=======\n", BFS_SIZE);
    return dev;
}
