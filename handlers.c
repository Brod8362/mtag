#include <gtk/gtk.h>
#include "handlers.h"

void file_selected(GtkTreeView* tree_view, GtkTreePath* tree_path, GtkTreeViewColumn* column, gpointer user_data) {
    printf("this runs");
}