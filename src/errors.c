#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

void try(ErrorCode error) {
    if (error == OK) return;
    if (error == DB_IO_ERROR) {
        puckxit();
    }
    if (error == NO_DATA) {
        print_no_data_message();
        return;
    }
    if (error == INVALID_DATA) {
        puckxit();
    }
    if (error == INVALID_INPUT) {
        puckxit();
    }
}

void puckxit() {
    fprintf(stderr, "Puck you, Verter!");
    exit(EXIT_FAILURE);
}

void print_no_data_message() { printf("NO DATA\n"); }