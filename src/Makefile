BUILD_PATH=../bin/
CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
EXTRAFLAGS=-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector 

all: check rebuild

build: clean simple_db simple_db_find not_simple_db

rebuild: clean_all simple_db simple_db_find not_simple_db clean

simple_db: clean simple_db.c data.c errors.c
	$(CC) $(CFLAGS) simple_db.c data.c errors.c -o  $(BUILD_PATH)simple_db 
	$(CC) $(CFLAGS) $(EXTRAFLAGS)  simple_db.c data.c errors.c -o  $(BUILD_PATH)simple_db_extra 

simple_db_find: clean simple_db.c data.c errors.c
	$(CC) $(CFLAGS) simple_db.c data.c errors.c -o  $(BUILD_PATH)simple_db_find 
	$(CC) $(CFLAGS) $(EXTRAFLAGS)  simple_db.c data.c errors.c -o  $(BUILD_PATH)simple_db_find_extra 

not_simple_db: clean simple_db.c data.c errors.c
	$(CC) $(CFLAGS) simple_db.c data.c errors.c -o  $(BUILD_PATH)not_simple_db 
	$(CC) $(CFLAGS) $(EXTRAFLAGS)  simple_db.c data.c errors.c -o  $(BUILD_PATH)not_simple_db_extra 

clean_all:
	rm -rf *.o *.a *.so $(BUILD_PATH)*
	
clean:
	rm -rf *.o *.a

check:
	clang-format -n *.c
	clang-format -n *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

run:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes $(BUILD_PATH)simple_db

run_no_valgrind:
	$(BUILD_PATH)simple_db_extra



