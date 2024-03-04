#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void puckxit();

// void my_fflush();

void main_menu_loop(char *db_path);

void check_db_path(char *db_path, const char *filename);

FILE *open_file(const char *db_path, const char *mode);

void add_record_to_file(Data *record_to_write, const char *db_path);

int find_record_by_date(Data *date_to_find, const char *db_path);

void max_sales(const char *db_path);

int get_records_count_in_file(const char *db_path);

void print_data(Data data);

void print_all(const char *db_path);

#endif