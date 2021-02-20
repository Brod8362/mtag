#include <sqlite3.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <assert.h>
#include "tags.h"

GtkApplication* app;

void destroy_all_children(GObject* widget) {
    if (GTK_IS_CONTAINER(widget)) {
        GList* children = gtk_container_get_children(GTK_CONTAINER(widget));
        GList* l;
        for (l = children; l != NULL; l = l->next) {
            gtk_widget_destroy(GTK_WIDGET(l->data));
        }
        g_list_free(children);
    }
}

int populate_directory_list(GObject* tree_obj, GtkTreeIter* parent, DIR* directory) {
    if (!GTK_IS_TREE_STORE(tree_obj)) {
        return 0;
    }
    GtkTreeStore* tree = GTK_TREE_STORE(tree_obj);
    struct dirent* de;
    size_t c = 0;
    while ((de = readdir(directory)) != NULL) {

        //skip current and previous directory
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }
        
        //if current file is a directory
        if (de->d_type==DT_DIR) {
            DIR* next_dir = opendir(de->d_name);
            if (next_dir == NULL) {
                fprintf(stderr, "failed to open directory %s\n", de->d_name);
                continue;
            }
            GtkTreeIter iter;
            gtk_tree_store_append(tree, &iter, parent);
            gtk_tree_store_set(tree, &iter, 0, de->d_name, 1, 0, -1);
            c += populate_directory_list(tree, &iter, next_dir);
            free(next_dir);
        }

        //if current file is a file
        if (de->d_type==DT_REG) {
            GtkTreeIter iter;
            gtk_tree_store_append(tree, &iter, parent);
            int file_tag_count = -1;
            gtk_tree_store_set(tree, &iter, 0, de->d_name, 1, file_tag_count, -1);
        }
        c++;
    }
    return c;
    //dir must be freed externally
}

int main(int argc, char* argv[]) {
    sqlite3* db;
    int res = sqlite3_open("test.db", &db);
    if (res != SQLITE_OK) {
        fprintf(stderr, "Cannot open database %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    const char* directory = ".";
    DIR* dr = opendir(directory);

    if (dr == NULL) {
        fprintf(stderr, "could not open directory %s\n", directory);
        return 1;
    }
    create_tables(db);
    prepare_statements(db);

    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/mtag.glade", NULL);

    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_builder_connect_signals(builder, NULL);

    GtkStatusbar* statusbar = GTK_STATUSBAR(gtk_builder_get_object(builder, "status_bar"));
    GObject* treemodel = gtk_builder_get_object(builder, "file_list_store");

    gtk_statusbar_push(statusbar, 0, "Scanning directories...");
    populate_directory_list(treemodel, NULL, dr);
    gtk_statusbar_push(statusbar, 0, "Directory scan complete.");

    gtk_widget_show(window);
    gtk_main();

    g_object_unref(builder);
    sqlite3_close(db);

    return 0;
}