#include <sqlite3.h>

void create_tables(sqlite3* db);

void prepare_statements(sqlite3* db);

typedef struct {
    char* filename;
    int tags[];
} TaggedFile;

typedef struct {
    int id;
    int category;
    char* name;
} Tag;

typedef struct {
    int id;
    char* name;
} TagCategory;
