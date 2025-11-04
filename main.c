#include <stdio.h>
#include <stdlib.h>
#include "cdllist.h"
#include "talloc.h"
#include "buffer.h"
#include "bufcache.h"
#include "inode.h"
#include "inocache.h"
#include "bfs.h"
#include "driver.h"
#include "inode.h"
#include "su_blk.h"

int main() {
    floppy = mkbfs("floppy.bfs");

    char store[BLOCK_SIZE];
    if (block_read(store, SUBLK_INDEX, floppy) == -1) {
        perr("Block Read has failed in reading the Super Block from the file.");
        return 1;
    }

    sublk* temp2 = read_sublk(store);
    print_sublk(temp2);

    puts("=====Exiting Main: Safe exiting. Deleting RAM=====");
    texit(0);
}