#include <gtk/gtk.h>
#include "handlers.h"

void file_selected(GtkTreeView* tree_view, GtkTreePath* tree_path, GtkTreeViewColumn* column, gpointer user_data) {
    struct FileInfoCollection* info = (struct FileInfoCollection*)user_data;

    GtkTreeSelection* selected = gtk_tree_view_get_selection(tree_view);
    GtkTreeModel* model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    const char* filename;

    //load selected row into iter
    gtk_tree_selection_get_selected(selected, &model, &iter);
    gtk_tree_model_get(model, &iter, 0, &filename, -1);

    //first, set the image label 
    gtk_label_set_text(info->filename_label, filename);

    //then set the image
    gtk_image_set_from_file(info->image, filename);
}