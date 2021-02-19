#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tags.h"

sqlite3_stmt* fetch_tag_count_stmt;
sqlite3_stmt* fetch_tags_stmt;
sqlite3_stmt* fetch_category_count_stmt;
sqlite3_stmt* fetch_categories_stmt;

sqlite3_stmt* create_tag_stmt;
sqlite3_stmt* rename_tag_stmt;
sqlite3_stmt* reassign_tag_category_stmt;
sqlite3_stmt* create_category_stmt;
sqlite3_stmt* rename_category_stmt;
sqlite3_stmt* get_file_tags_stmt;
sqlite3_stmt* add_tag_file_stmt;
sqlite3_stmt* remove_tag_file_stmt;

// ###### FILE FUNCTIONS ######

void remove_file_tag(sqlite3* db, TaggedFile* file, Tag* tag) {
    //name comes 2nd
    sqlite3_bind_int(remove_tag_file_stmt, 1, tag->id);
    sqlite3_bind_text(remove_tag_file_stmt, 2, file->filename, -1, 0);
    int res = sqlite3_step(remove_tag_file_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "failed removing file tag:%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(remove_tag_file_stmt);
}

void add_file_tag(sqlite3* db, TaggedFile* file, Tag* tag) {
    //this function will reassign the file pointer to include the new tag added
    sqlite3_bind_text(add_tag_file_stmt, 1, file->filename, -1, 0);
    sqlite3_bind_int(add_tag_file_stmt, 2, tag->id);
    int res = sqlite3_step(add_tag_file_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "failed to add tag to file:%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(add_tag_file_stmt);
    //if there's segfaults or memory leaks, look here, this is probably a bad idea
    //HEY SCORE IF YOU WANT TO BUG ME ABOUT SOMETHING, START HERE!
    get_file_tags(db, file->filename, file);
}

void get_file_tags(sqlite3* db, const char* filename, TaggedFile* output) {
    sqlite3_bind_text(get_file_tags_stmt, 1, filename, -1, 0);
    int res = sqlite3_step(get_file_tags_stmt);
    int count = sqlite3_changes(db);
    int tags[count];
    int i = 0;
    do {
        tags[i] = sqlite3_column_int(get_file_tags_stmt, 0);
        res = sqlite3_step(get_file_tags_stmt);
    } while (res == SQLITE_ROW);
    output = (TaggedFile*) malloc(sizeof(const char*) + sizeof(int)*(count+1));
    output->filename=filename;
    output->tag_count=count;
    output->tags=tags;
    sqlite3_reset(get_file_tags_stmt);
}

// ###### CATEGORY FUNCTIONS ######

void rename_category(sqlite3* db, TagCategory* category, const char* new_name) {
    sqlite3_bind_text(rename_category_stmt, 1, new_name, -1, 0);
    sqlite3_bind_int(rename_category_stmt, 2, category->id);
    int res = sqlite3_step(rename_category_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "failed to rename category:%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(rename_category_stmt);
}

void create_category(sqlite3* db, const char* name) {
    sqlite3_bind_text(create_category_stmt, 1, name, -1, 0);
    int res = sqlite3_step(create_category_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "failed to create category:%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(create_category_stmt);
}

int retrieve_category_count(sqlite3* db) {
    sqlite3_step(fetch_category_count_stmt);
    int r = sqlite3_column_int(fetch_category_count_stmt, 0);
    sqlite3_reset(fetch_category_count_stmt);
    return r;
}

void retrieve_categories(sqlite3* db, TagCategory* categories[], int size) {
    int i = 0;
    int step;
    do {
        if (i >= size) break;
        step = sqlite3_step(fetch_categories_stmt);
        TagCategory category = {
            .id = sqlite3_column_int(fetch_categories_stmt, 0),
            .name = sqlite3_column_text(fetch_categories_stmt, 1) //todo this is not working currently
        };
        categories[i++] = &category;
    } while (step == SQLITE_ROW);
    sqlite3_reset(fetch_categories_stmt);
}

// ###### TAG FUNCTIONS ######

void rename_tag(sqlite3* db, Tag* tag, const char* new_name) {
    sqlite3_bind_text(rename_tag_stmt, 1, new_name, -1, 0);
    sqlite3_bind_int(rename_tag_stmt, 2, tag->id);
    int res = sqlite3_step(rename_tag_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "failed to rename tag:%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(rename_tag_stmt);
}

void create_tag(sqlite3* db, const char* name, TagCategory* category) {
    sqlite3_bind_int(create_tag_stmt, 1, category->id);
    sqlite3_bind_text(create_tag_stmt, 2, name, -1, 0);
    int res = sqlite3_step(create_tag_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "failed to create tag:%s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(create_tag_stmt);
}

void change_tag_category(sqlite3* db, Tag* tag, TagCategory* category) {
    //may pass in NULL pointer to unset the category 
    int cat_id = -1;
    if (category != NULL) {
        cat_id = category->id;
    }
    sqlite3_bind_int(reassign_tag_category_stmt, 1, cat_id);
    sqlite3_bind_int(reassign_tag_category_stmt, 2, tag->id);
    int res = sqlite3_step(reassign_tag_category_stmt);
    if (res != SQLITE_OK) {
        fprintf(stderr, "can't change tag category: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_reset(reassign_tag_category_stmt);
}

int retrieve_tag_count(sqlite3* db) {
    sqlite3_step(fetch_tag_count_stmt);
    int r = sqlite3_column_int(fetch_tag_count_stmt, 0);
    sqlite3_reset(fetch_tag_count_stmt);
    return r;
}

void retrieve_tags(sqlite3* db, Tag* tags[], int size) {
    int i = 0;
    int step;
    do {
        if (i >= size) break;
        step = sqlite3_step(fetch_tags_stmt);
        Tag tag = {
            .id = sqlite3_column_int(fetch_tags_stmt, 0),
            .category = sqlite3_column_int(fetch_tags_stmt, 1),
            .name = sqlite3_column_text(fetch_tags_stmt, 2)
        };
        tags[i++] = &tag;
    } while (step == SQLITE_ROW);
    sqlite3_reset(fetch_tags_stmt);
}

// ###### MISC FUNCTIONS ######

void create_tables(sqlite3* db) {
    const char* tables[] = {
        "CREATE TABLE tags (id INTEGER PRIMARY KEY AUTOINCREMENT, category INTEGER, name STRING NOT NULL)",
        "CREATE TABLE categories (id INTEGER PRIMARY KEY AUTOINCREMENT, name STRING NOT NULL)",
        "CREATE TABLE files (id INTEGER PRIMARY KEY AUTOINCREMENT, name STRING NOT NULL, tag INTEGER NOT NULL)"
    };
    char* errmsg;
    for (int i = 0; i < 3; i++) {
        int res = sqlite3_exec(db, tables[i], NULL, 0, &errmsg);
        if (res != SQLITE_OK && res != SQLITE_ACCESS_EXISTS) {
            fprintf(stderr, "Failed creating table: %s\n", errmsg);
        }
    }
}

void prepare_statements(sqlite3* db) {
    sqlite3_prepare_v2(db, "SELECT tag FROM files WHERE name=?", -1, &get_file_tags_stmt, 0);
    sqlite3_prepare_v2(db, "SELECT COUNT(id) FROM tags", -1, &fetch_tag_count_stmt, 0);
    sqlite3_prepare_v2(db, "SELECT * from tags", -1, &fetch_tags_stmt, 0);
    sqlite3_prepare_v2(db, "SELECT COUNT(id) FROM categories", -1, &fetch_category_count_stmt, 0);
    sqlite3_prepare_v2(db, "SELECT * FROM categories", -1, &fetch_categories_stmt, 0);

    sqlite3_prepare_v2(db, "INSERT INTO tags (category, name) VALUES (?, ?)", -1, &create_tag_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE tags SET name=? WHERE id=?", -1, &rename_tag_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE tags SET category=? WHERE id=?", -1, &reassign_tag_category_stmt, 0);
    sqlite3_prepare_v2(db, "INSERT INTO categories (name) VALUES (?)", -1, &create_category_stmt, 0);
    sqlite3_prepare_v2(db, "UPDATE categories SET name=? WHERE id=?", -1, &rename_category_stmt, 0);
    sqlite3_prepare_v2(db, "DELETE FROM files WHERE id=? AND name=?", -1, &remove_tag_file_stmt, 0);
    sqlite3_prepare_v2(db, "INSERT INTO files (name, tag) VALUES (?, ?)", -1, &add_tag_file_stmt, 0);
}