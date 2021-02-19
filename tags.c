#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include "tags.h"

sqlite3_stmt* fetch_file_tags;
sqlite3_stmt* fetch_tag_count;
sqlite3_stmt* fetch_tags;
sqlite3_stmt* fetch_category_count;
sqlite3_stmt* fetch_categories;

sqlite3_stmt* get_file_tags;
sqlite3_stmt* create_tag_stmt;
sqlite3_stmt* rename_tag_stmt;
sqlite3_stmt* reassign_tag_category_stmt;
sqlite3_stmt* create_category_stmt;
sqlite3_stmt* rename_category_stmt;
sqlite3_stmt* add_tag_file_stmt;
sqlite3_stmt* remove_tag_file_stmt;

void create_category(sqlite3* db, const char* name) {
    sqlite3_bind_text(create_category_stmt, 0, name, -1, 0);
    int res = sqlite3_step(create_category_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(create_category_stmt);
}

int retrieve_category_count(sqlite3* db) {
    sqlite3_step(fetch_category_count);
    int r = sqlite3_column_int(fetch_category_count, 0);
    sqlite3_reset(fetch_category_count);
    return r;
}

void retrieve_categories(sqlite3* db, TagCategory* categories[], int size) {
        int i = 0;
    int step;
    do {
        if (i >= size) break;
        step = sqlite3_step(fetch_categories);
        TagCategory category = {
            .id = sqlite3_column_int(fetch_categories, 0),
            .name = sqlite3_column_text(fetch_categories, 1)
        };
        categories[i++] = &category;
    } while (step == SQLITE_ROW);
    sqlite3_reset(fetch_categories);
}

void create_tag(sqlite3* db, const char* name, TagCategory* category) {
    sqlite3_bind_int(create_tag_stmt, 0, category->id);
    sqlite3_bind_text(create_tag_stmt, 1, name, -1, 0);
    int res = sqlite3_step(create_tag_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(create_tag_stmt);
}

int retrieve_tag_count(sqlite3* db) {
    sqlite3_step(fetch_tag_count);
    int r = sqlite3_column_int(fetch_tag_count, 0);
    sqlite3_reset(fetch_tag_count);
    return r;
}

void retrieve_tags(sqlite3* db, Tag* tags[], int size) {
    int i = 0;
    int step;
    do {
        if (i >= size) break;
        step = sqlite3_step(fetch_tags);
        Tag tag = {
            .id = sqlite3_column_int(fetch_tags, 0),
            .category = sqlite3_column_int(fetch_tags, 1),
            .name = sqlite3_column_text(fetch_tags, 2)
        };
        tags[i++] = &tag;
    } while (step == SQLITE_ROW);
    sqlite3_reset(fetch_tags);
}

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
    sqlite3_prepare_v2(db, "SELECT tag FROM files WHERE name=?", -1, &fetch_file_tags, 0);
    sqlite3_prepare_v2(db, "SELECT COUNT(id) FROM tags", -1, &fetch_tag_count, 0);
    sqlite3_prepare_v2(db, "SELECT * from tags", -1, &fetch_tags, 0);
    sqlite3_prepare_v2(db, "SELECT COUNT(id) FROM categories", -1, &fetch_category_count, 0);
    sqlite3_prepare_v2(db, "SELECT * FROM categories", -1, &fetch_categories, 0);
    sqlite3_prepare_v2(db, "INSERT INTO tags (category, name) VALUES (?, ?)", -1, &create_tag_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE tags SET name=? WHERE id=?", -1, &rename_tag_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE tags SET category=? WHERE id=?", -1, &reassign_tag_category_stmt, 0);
    sqlite3_prepare_v2(db, "INSERT INTO categories (name) VALUES (?)", -1, &create_category_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE categories SET name=? WHERE id=?", -1, &rename_category_stmt, 0);
    sqlite3_prepare_v2(db, "DELETE FROM files WHERE id=?", -1, &remove_tag_file_stmt, 0);
    sqlite3_prepare_v2(db, "INSERT INTO files (name, tag) VALUES (?, ?)", -1, &add_tag_file_stmt, 0);
}