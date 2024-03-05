#ifndef ERRORS_H
#define ERRORS_H

typedef enum { OK, DB_IO_ERROR, NO_DATA, INVALID_DATA, INVALID_INPUT } ErrorCode;


void puckxit();

void try(ErrorCode error);
void print_no_data_message();
#endif