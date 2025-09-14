#ifndef HLPRCD_C
#define HLPRCD_C

#include "helpercode.h"
#include <stdio.h>
#include <string.h>

int std_print_enabled = 1;

// TODO: Add error checking
int plog(char * str) {
    FILE * f = fopen("build/fslog.log", "a");
    fprintf(f, str);    
    if (std_print_enabled) 
        printf(str);
    return strlen(str);
}

int perr(char * str) {
    FILE * f = fopen("build/fserror.err", "a");
    fprintf(f, str);
    if (std_print_enabled) 
        perror(str);
    return strlen(str);
}

void disable_std_print() {
    std_print_enabled = 0;
}

#endif