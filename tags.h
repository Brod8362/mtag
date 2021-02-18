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


int retrieve_tag_count(sqlite3* db);

void retrieve_tags(sqlite3* db, Tag* tags[], int size);

void create_tables(sqlite3* db);

void prepare_statements(sqlite3* db);
