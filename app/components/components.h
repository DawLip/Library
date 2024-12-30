#include <gtk/gtk.h>

GtkWidget *Menu(GtkWidget *parent);

GtkWidget *Hierarchy(GtkWidget *parent);
GtkWidget *HierarchyUsers(GtkWidget *parent);
GtkWidget *HierarchyBooks(GtkWidget *parent);
void Hierarchy_rerender();

GtkWidget *Workspace(GtkWidget *parent);
void Workspace_rerender();