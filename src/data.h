#ifndef DATA_H
#define DATA_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "errors.h"

#define MAX_RECORDS 1000000
#define MAX_NAME_LEN 100
#define BUFFER 256
#define FILENAME "../data/database.txt"

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    char name[MAX_NAME_LEN];
    int price;
} Data;

typedef struct {
    char name[MAX_NAME_LEN];
    int amount;
} Name_Amount;

ErrorCode isValidCommand(char *command);

ErrorCode main_menu_loop(char *db_path);

ErrorCode check_db_path(char *db_path, const char *filename);

FILE *open_file(const char *db_path, const char *mode);

ErrorCode add_record_to_file(Data *record_to_write, const char *db_path);

ErrorCode find_record_by_date(Data *date_to_find, const char *db_path);

ErrorCode aggregate_sales_by_name(Name_Amount *name_amount, int *name_amount_cnt, int records_cnt,
                                  const char *db_path);

ErrorCode max_sales(const char *db_path);

ErrorCode parse_data_from_string(char *line, Data *data);

int get_records_count_in_file(const char *db_path);

char *stringToLower(char *string);

void set_current_time(Data *data);

void print_data(Data data);

void print_all(const char *db_path);

#endif