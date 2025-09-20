#ifndef HLPRCD_C
#define HLPRCD_C

#include "helpercode.h"
#include <stdio.h>
#include <string.h>

#define LOG_STR_FORMAT "%s\n"
int std_print_enabled = 1;


// TODO: Add error checking
int plog(char * str) {
    FILE * f = fopen("build/fslog.log", "a");
    fprintf(f, LOG_STR_FORMAT, str);    
    if (std_print_enabled) 
        printf(LOG_STR_FORMAT, str);
    return strlen(str);
}

int perr(char * str) {
    FILE * f = fopen("build/fserror.err", "a");
    fprintf(f, LOG_STR_FORMAT, str);
    if (std_print_enabled) 
        perror(str);
    return strlen(str);
}

void disable_std_print() {
    std_print_enabled = 0;
}

#endif