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
GtkWidget *WorkspaceUsers(GtkWidget *parent);
GtkWidget *WorkspaceBooks(GtkWidget *parent);
void Workspace_rerender();
void on_Input_change(GtkEntry *entry, char* dest);

GtkWidget *TopBar(GtkWidget *parent);