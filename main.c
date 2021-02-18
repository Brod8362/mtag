#include <sqlite3.h>
#include <stdio.h>

int main() {
    printf("test\n");
    sqlite3** db;
    int handle = sqlite3_open("test.db", db);
}