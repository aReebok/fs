#ifndef HLPRCD_C
#define HLPRCD_C

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "util.h"

#define LOG_STR_FORMAT "%s\n"
int std_print_enabled = 1;

int plog(char * str) {
    FILE * f = fopen("build/fslog.log", "a");
    if (!f) {
        fprintf(stderr, "plog: failed to open log file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(f, LOG_STR_FORMAT, str);    
    if (std_print_enabled) 
        printf(LOG_STR_FORMAT, str);
    return strlen(str);
}

int perr(char * str) {
    FILE * f = fopen("build/fserror.err", "a");
    if (!f) {
        fprintf(stderr, "perr: failed to open log file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(f, LOG_STR_FORMAT, str);
    if (std_print_enabled) 
        perror(str);
    return strlen(str);
}

void disable_std_print() {
    std_print_enabled = 0;
}

#endif