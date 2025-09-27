#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "bfs_drv.h"
#include "util.h"
#include "talloc.h"

#define ERROR() \
    perr("Talloc failed: BFS_DEVICE could not be created");\
    return NULL;

bfs * floppy;

bfs * initialize_bfs(const char * bfs_path) {


    bfs * dev = talloc(sizeof(*dev));

    if(dev == NULL) {
        ERROR(); // TODO: Test in a safe environment whether the semi-colon or braces are necessary
    }

    int f = creat(bfs_path, 0644);

    if (ftruncate(f, BFS_SIZE) == -1) {
        ERROR();
    }

    close(f);
    plog("Successfully created BFS");
    return dev;
}