#include <gtk/gtk.h>

struct FileInfoCollection {
    GtkImage* image;
    GtkLabel* filename_label;
    GtkListStore* tag_list;
    GtkStatusbar* statusbar;
};

void file_selected(GtkTreeView* tree_view, GtkTreePath* tree_path, GtkTreeViewColumn* column, gpointer user_data);