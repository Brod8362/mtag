#include <sqlite3.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>
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

void populate_directory_list(GtkWidget* tree, DIR* directory) {
    if (!GTK_IS_TREE_VIEW(tree)) {
        return;
    }
    destroy_all_children(tree);
    struct dirent* de;
    while ((de = readdir(directory)) != NULL) {
        printf("%s\n", de->d_name);
    }
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

    populate_directory_list(GTK_WIDGET(gtk_builder_get_object(builder, "file_list_tree")), dr);

    gtk_widget_show(window);
    gtk_main();

    g_object_unref(builder);
    sqlite3_close(db);

    return 0;
}