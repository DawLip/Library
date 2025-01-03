#pragma once

#include <gtk/gtk.h>

GtkWidget *Welcome(GtkWidget *parent);

GtkWidget *Menu(GtkWidget *parent);

GtkWidget *Hierarchy(GtkWidget *parent);
GtkWidget *HierarchyUsers(GtkWidget *parent);
GtkWidget *HierarchyBooks(GtkWidget *parent);
extern GtkWidget *hierarchySearcher;
void Hierarchy_rerender();

GtkWidget *Workspace(GtkWidget *parent);
void Workspace_rerender();

GtkWidget *TopBar(GtkWidget *parent);