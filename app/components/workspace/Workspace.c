#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *workspaceWrapper;
GtkWidget *workspace;
GtkWidget *workspaceParent;

GtkWidget *workspaceBooks;

void on_Input_change(GtkEntry *entry, char* dest) {
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    strcpy(dest, text);
    
    Hierarchy_rerender();
}

void Workspace_render(){
  if(dataUI->currWindow == USERS) workspace=WorkspaceUsers(workspaceWrapper);
  else if(dataUI->currWindow == BOOKS) workspace=WorkspaceBooks(workspaceWrapper);
}

void Workspace_rerender(){
  gtk_box_remove(workspaceWrapper, workspace);
  
  Workspace_render();
}

GtkWidget *Workspace(GtkWidget *p) {
    if(workspaceParent==NULL) workspaceParent = p;

    workspaceWrapper = Div(workspaceParent, "WorkspaceWrapper", "v", "vh", 0);
    Workspace_render();
}