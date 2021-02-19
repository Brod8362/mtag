#include <sqlite3.h>

typedef struct {
    const char* filename;
    int tags[];
} TaggedFile;

typedef struct {
    int id;
    int category;
    const unsigned char* name;
} Tag;

typedef struct {
    int id;
    const unsigned char* name;
} TagCategory;

//Category related

void create_category(sqlite3* db, const char* name);

int retrieve_category_count(sqlite3* db);

void retrieve_categories(sqlite3* db, TagCategory* categories[], int size);

void create_tag(sqlite3* db, const char* name, TagCategory* category);

//Tag related

int retrieve_tag_count(sqlite3* db);

void retrieve_tags(sqlite3* db, Tag* tags[], int size);

void create_tables(sqlite3* db);

void prepare_statements(sqlite3* db);
