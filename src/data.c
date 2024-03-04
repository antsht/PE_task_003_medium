#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void puckxit() {
    fprintf(stderr, "Puck you, Verter!");
    exit(EXIT_FAILURE);
}

char *stringToLower(char *string) {
    for (char *c = string; *c; c++) *c = tolower(*c);
    return string;
}
/*void my_fflush() {
    char c;
    while ((c = getchar()) != '\n' && c != -1) {
        ;
    }
}*/
void main_menu_loop(char *db_path) {
    int stop = 0;
    while (!stop) {
        char command[BUFFER] = {0};

        if (scanf("%10s", command) != 1 ||
            !(strcmp(command, "SHOW") == 0 || strcmp(command, "ADD") == 0 || strcmp(command, "FIND") == 0 ||
              strcmp(command, "MAX") == 0 || strcmp(command, "EXIT") == 0))
            puckxit();

        if (strcmp(command, "SHOW") == 0) {
            print_all(db_path);
        } else if (strcmp(command, "ADD") == 0) {
            // my_fflush();
            Data data;
            if (scanf("%100s %10d", data.name, &data.price) != 2) puckxit();
            add_record_to_file(&data, db_path);

        } else if (strcmp(command, "FIND") == 0) {
            // my_fflush();
            Data data;
            if (scanf("%2d.%2d.%4d", &data.day, &data.month, &data.year) != 3) puckxit();
            int records_found = find_record_by_date(&data, db_path);
            if (records_found == 0) printf("NO DATA\n");
        } else if (strcmp(command, "MAX") == 0) {
            // my_fflush();
            max_sales(db_path);
        } else if (strcmp(command, "EXIT") == 0)
            stop = 1;
    }
}
void check_db_path(char *db_path, const char *filename) {
    strcpy(db_path, filename);
    FILE *pfile = open_file(db_path, "a");
    fclose(pfile);
    if (access(db_path, F_OK) != 0) {
        puckxit();
    }
}

FILE *open_file(const char *db_path, const char *mode) {
    FILE *pfile = fopen(db_path, mode);
    if (pfile == NULL) {
        puckxit();
    }
    return pfile;
}

void add_record_to_file(Data *data, const char *db_path) {
    FILE *pfile = open_file(db_path, "a+");
    time_t rawtime;
    struct tm *tm;
    time(&rawtime);
    tm = localtime(&rawtime);

    data->day = tm->tm_mday;
    data->month = tm->tm_mon + 1;
    data->year = tm->tm_year + 1900;
    data->hour = tm->tm_hour;
    data->minute = tm->tm_min;
    data->second = tm->tm_sec;
    fprintf(pfile, "%02d.%02d.%04d %02d:%02d:%02d %s %d\n", data->day, data->month, data->year, data->hour,
            data->minute, data->second, data->name, data->price);
    fclose(pfile);
}

int find_record_by_date(Data *date_to_find, const char *db_path) {
    FILE *pfile = open_file(db_path, "r");
    int records_found = 0;
    char line[BUFFER];
    while (fgets(line, BUFFER - 1, pfile) != NULL) {
        Data data;
        sscanf(line, "%2d.%2d.%4d %2d:%2d:%2d %100s %10d", &data.day, &data.month, &data.year, &data.hour,
               &data.minute, &data.second, data.name, &data.price);
        if (data.day == date_to_find->day && data.month == date_to_find->month &&
            data.year == date_to_find->year) {
            print_data(data);
            records_found++;
        }
    }
    fclose(pfile);
    return records_found;
}

void max_sales(const char *db_path) {
    int records_cnt = get_records_count_in_file(db_path);
    if (records_cnt == 0) {
        puckxit();
    }
    Name_Amount *name_amount = (Name_Amount *)malloc(records_cnt * sizeof(Name_Amount));
    int name_amount_cnt = 0;

    FILE *pfile = open_file(db_path, "r");
    for (int i = 0; i < records_cnt; ++i) {
        char line[BUFFER];
        fgets(line, BUFFER - 1, pfile);
        Data data;
        sscanf(line, "%2d.%2d.%4d %2d:%2d:%2d %100s %10d", &data.day, &data.month, &data.year, &data.hour,
               &data.minute, &data.second, data.name, &data.price);
        int name_exists = 0;
        for (int j = 0; j < name_amount_cnt; ++j) {
            if (strcmp(stringToLower(data.name), name_amount[j].name) == 0) {
                name_amount[j].amount += data.price;
                name_exists = 1;
                break;
            }
        }
        if (!name_exists) {
            strcpy(name_amount[name_amount_cnt].name, stringToLower(data.name));
            name_amount[name_amount_cnt].amount = data.price;
            name_amount_cnt++;
        }
    }
    fclose(pfile);

    int max_amount = 0;
    for (int i = 0; i < name_amount_cnt; ++i) {
        if (name_amount[i].amount > max_amount) max_amount = name_amount[i].amount;
    }
    for (int i = 0; i < name_amount_cnt; ++i) {
        if (name_amount[i].amount == max_amount) {
            *(name_amount[i].name) = toupper(*(name_amount[i].name));
            printf("%s %d\n", name_amount[i].name, name_amount[i].amount);
        }
    }
    free(name_amount);
}

int get_records_count_in_file(const char *db_path) {
    FILE *pfile = open_file(db_path, "r");
    int cnt = 0;
    char tmp[BUFFER];
    while (fgets(tmp, BUFFER, pfile) != NULL) {
        cnt++;
    }
    fclose(pfile);
    return cnt;
}

void print_data(Data data) {
    printf("%02d.%02d.%04d %02d:%02d:%02d %s %d\n", data.day, data.month, data.year, data.hour, data.minute,
           data.second, data.name, data.price);
}

void print_all(const char *db_path) {
    int records_cnt = get_records_count_in_file(db_path);
    FILE *pfile = open_file(db_path, "r");
    for (int i = 0; i < records_cnt; ++i) {
        char line[BUFFER];
        fgets(line, BUFFER - 1, pfile);
        Data data;
        sscanf(line, "%2d.%2d.%4d %2d:%2d:%2d %100s %10d", &data.day, &data.month, &data.year, &data.hour,
               &data.minute, &data.second, data.name, &data.price);
        print_data(data);
    }
    fclose(pfile);
}
