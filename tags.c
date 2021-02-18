#include <sqlite3.h>
#include <string.h>
#include <stdio.h>

sqlite3_stmt* create_tag_stmt;
sqlite3_stmt* rename_tag_stmt;
sqlite3_stmt* reassign_tag_category_stmt;
sqlite3_stmt* create_category_stmt;
sqlite3_stmt* rename_category_stmt;
sqlite3_stmt* tag_file_stmt;
sqlite3_stmt* update_file_tags_stmt;

void create_tables(sqlite3* db) {
    const char* tables[] = {
        "CREATE TABLE tags (id INTEGER PRIMARY KEY AUTOINCREMENT, category INTEGER, name STRING NOT NULL)",
        "CREATE TABLE categories (id INTEGER PRIMARY KEY AUTOINCREMENT, name STRING NOT NULL)",
        "CREATE TABLE files (name STRING NOT NULL, tag INTEGER NOT NULL)"
    };
    char* errmsg;
    for (int i = 0; i < 3; i++) {
        if (sqlite3_exec(db, tables[i], NULL, 0, &errmsg) != SQLITE_OK) {
            fprintf(stderr, "Failed creating table: %s\n", errmsg);
        }
    }
}

void prepare_statements(sqlite3* db) {
}