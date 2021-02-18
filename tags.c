#include <sqlite3.h>
#include <string.h>
#include <stdio.h>

sqlite3_stmt* create_tag_stmt;
sqlite3_stmt* rename_tag_stmt;
sqlite3_stmt* reassign_tag_category_stmt;
sqlite3_stmt* create_category_stmt;
sqlite3_stmt* rename_category_stmt;
sqlite3_stmt* add_tag_file_stmt;
sqlite3_stmt* remove_tag_file_stmt;

void create_tables(sqlite3* db) {
    const char* tables[] = {
        "CREATE TABLE tags (id INTEGER PRIMARY KEY AUTOINCREMENT, category INTEGER, name STRING NOT NULL)",
        "CREATE TABLE categories (id INTEGER PRIMARY KEY AUTOINCREMENT, name STRING NOT NULL)",
        "CREATE TABLE files (id INTEGER PRIMARY KEY AUTOINCREMENT, name STRING NOT NULL, tag INTEGER NOT NULL)"
    };
    char* errmsg;
    for (int i = 0; i < 3; i++) {
        if (sqlite3_exec(db, tables[i], NULL, 0, &errmsg) != SQLITE_OK) {
            fprintf(stderr, "Failed creating table: %s\n", errmsg);
        }
    }
}

void prepare_statements(sqlite3* db) {
    sqlite3_prepare_v2(db, "INSERT INTO tags VALUES (?, ?)", -1, &create_tag_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE tags SET name=? WHERE id=?", -1, &rename_tag_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE tags SET category=? WHERE id=?", -1, &reassign_tag_category_stmt, 0);
    sqlite3_prepare_v2(db, "INSERT INTO categories VALUES (?)", -1, &create_category_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE categories SET name=? WHERE id=?", -1, &rename_category_stmt, 0);
    sqlite3_prepare_v2(db, "DELETE FROM files WHERE id=?", -1, &remove_tag_file_stmt, 0);
    sqlite3_prepare_v2(db, "INSERT INTO files VALUES (?, ?)", -1, &add_tag_file_stmt, 0);
}