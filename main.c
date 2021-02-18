#include <sqlite3.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include "tags.h"

int main() {
    sqlite3* db;
    int res = sqlite3_open("test.db", &db);
    if (res != SQLITE_OK) {
        fprintf(stderr, "Cannot open database %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    create_tables(db);
    sqlite3_close(db);
}