#include "data.h"

char *stringToLower(char *string) {
    for (char *c = string; *c; c++) *c = tolower(*c);
    return string;
}

ErrorCode isValidCommand(char *command) {
    if (strcmp(command, "SHOW") == 0 || strcmp(command, "ADD") == 0 || strcmp(command, "FIND") == 0 ||
        strcmp(command, "MAX") == 0 || strcmp(command, "EXIT") == 0)
        return INVALID_INPUT;
    return OK;
}

ErrorCode main_menu_loop(char *db_path) {
    int stop = 0;
    ErrorCode error = OK;

    while (!stop) {
        char command[BUFFER] = {0};
        if (scanf("%10s", command) != 1 || !isValidCommand(command)) {
            return INVALID_INPUT;
        }

        if (strcmp(command, "SHOW") == 0) {
            print_all(db_path);
        }

        if (strcmp(command, "ADD") == 0) {
            Data data;
            if (scanf("%100s %10d", data.name, &data.price) != 2 || data.price < 0) return INVALID_INPUT;
            if ((error = add_record_to_file(&data, db_path))) return error;
        }

        if (strcmp(command, "FIND") == 0) {
            Data data;
            if (scanf("%2d.%2d.%4d", &data.day, &data.month, &data.year) != 3 || data.day < 1 ||
                data.month < 1 || data.year < 1)
                return INVALID_INPUT;
            if ((error = find_record_by_date(&data, db_path))) return error;
        }
        if (strcmp(command, "MAX") == 0) {
            if ((error = max_sales(db_path))) return error;
        }

        if (strcmp(command, "EXIT") == 0) {
            stop = 1;
        }
    }
    return error;
}
ErrorCode check_db_path(char *db_path, const char *filename) {
    strcpy(db_path, filename);
    FILE *pfile = open_file(db_path, "a");
    fclose(pfile);
    if (access(db_path, F_OK) != 0) {
        return DB_IO_ERROR;
    }
    return OK;
}

FILE *open_file(const char *db_path, const char *mode) {
    FILE *pfile = fopen(db_path, mode);
    if (pfile == NULL) {
        puckxit();
    }
    return pfile;
}

void set_current_time(Data *data) {
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
}

ErrorCode add_record_to_file(Data *data, const char *db_path) {
    FILE *pfile = open_file(db_path, "a+");
    if (pfile == NULL) return DB_IO_ERROR;

    set_current_time(data);
    fprintf(pfile, "%02d.%02d.%04d %02d:%02d:%02d %s %d\n", data->day, data->month, data->year, data->hour,
            data->minute, data->second, data->name, data->price);
    fclose(pfile);
    return OK;
}

ErrorCode parse_data_from_string(char *line, Data *data) {
    if (sscanf(line, "%2d.%2d.%4d %2d:%2d:%2d %100s %10d", &data->day, &data->month, &data->year, &data->hour,
               &data->minute, &data->second, data->name, &data->price) != 8)
        return INVALID_DATA;
    return OK;
}

ErrorCode find_record_by_date(Data *date_to_find, const char *db_path) {
    FILE *pfile = open_file(db_path, "r");
    if (pfile == NULL) return DB_IO_ERROR;
    int records_found = 0;
    char line[BUFFER];
    while (fgets(line, BUFFER - 1, pfile) != NULL) {
        Data data;
        parse_data_from_string(line, &data);
        if (data.day == date_to_find->day && data.month == date_to_find->month &&
            data.year == date_to_find->year) {
            print_data(data);
            records_found++;
        }
    }
    fclose(pfile);
    if (records_found == 0) return NO_DATA;
    return OK;
}

ErrorCode aggregate_sales_by_name(Name_Amount *name_amount, int *name_amount_cnt, int records_cnt,
                                  const char *db_path) {
    FILE *pfile = open_file(db_path, "r");
    if (pfile == NULL) return DB_IO_ERROR;
    for (int i = 0; i < records_cnt; ++i) {
        ErrorCode error;
        char line[BUFFER];
        fgets(line, BUFFER - 1, pfile);
        Data data;
        if ((error = parse_data_from_string(line, &data))) return error;
        int name_exists = 0;
        for (int j = 0; j < *name_amount_cnt; ++j) {
            if (strcmp(stringToLower(data.name), name_amount[j].name) == 0) {
                name_amount[j].amount += data.price;
                name_exists = 1;
                break;
            }
        }
        if (!name_exists) {
            strcpy(name_amount[*name_amount_cnt].name, stringToLower(data.name));
            name_amount[*name_amount_cnt].amount = data.price;
            (*name_amount_cnt)++;
        }
    }
    fclose(pfile);
    return OK;
}

ErrorCode max_sales(const char *db_path) {
    int records_cnt = get_records_count_in_file(db_path);
    if (records_cnt == 0) {
        return NO_DATA;
    }

    Name_Amount *name_amount = (Name_Amount *)malloc(records_cnt * sizeof(Name_Amount));
    int name_amount_cnt = 0;

    aggregate_sales_by_name(name_amount, &name_amount_cnt, records_cnt, db_path);

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
    return OK;
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
        parse_data_from_string(line, &data);
        print_data(data);
    }
    fclose(pfile);
}
