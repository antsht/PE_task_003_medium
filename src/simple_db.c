#include "data.h"

int main(int argc, char* argv[]) {
    char db_path[BUFFER] = {0};
    check_db_path(db_path, argc == 2 ? argv[1] : FILENAME);
    main_menu_loop(db_path);
    return 0;
}
