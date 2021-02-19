#include <sqlite3.h>

typedef struct {
    int id;
    int category;
    const unsigned char* name;
} Tag;

typedef struct {
    int id;
    const unsigned char* name;
} TagCategory;

typedef struct {
    const char* filename;
    int tag_count;
    int* tags;
} TaggedFile;

//File related

void remove_file_tag(sqlite3* db, TaggedFile* file, Tag* tag);

void add_file_tag(sqlite3* db, TaggedFile* file, Tag* tag);

void get_file_tags(sqlite3* db, const char* filename, TaggedFile* output);

//Category related

void rename_category(sqlite3* db, TagCategory* category, const char* new_name);

void create_category(sqlite3* db, const char* name);

int retrieve_category_count(sqlite3* db);

void retrieve_categories(sqlite3* db, TagCategory* categories[], int size);

void create_tag(sqlite3* db, const char* name, TagCategory* category);

//Tag related

void rename_tag(sqlite3* db, Tag* tag, const char* new_name);

void change_tag_category(sqlite3* db, Tag* tag, TagCategory* category);

int retrieve_tag_count(sqlite3* db);

void retrieve_tags(sqlite3* db, Tag* tags[], int size);

//Misc

void create_tables(sqlite3* db);

void prepare_statements(sqlite3* db);
