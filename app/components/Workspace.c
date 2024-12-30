#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

GtkWidget *workspaceWrapper;
GtkWidget *workspace;
GtkWidget *workspaceParent;

GtkWidget *workspaceUsers;

GtkWidget *workspaceBooks;
GtkWidget *WorkspaceBooksName;
GtkWidget *WorkspaceBooksAuthor;

void on_activate(GtkEntry *entry, char* dest) {
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
    strcpy(dest, text);
    
    Hierarchy_rerender();
}

GtkWidget *Input(GtkWidget *parent, char* value) {
    GtkWidget *entry = gtk_entry_new();
    
    g_signal_connect(entry, "activate", G_CALLBACK(on_activate), value);
    gtk_editable_set_text(entry, value);

    gtk_box_append(GTK_BOX(parent), entry);
}

GtkWidget *WorkspaceUsers(GtkWidget *parent) {
    GtkWidget *workspaceUsers = Div(parent, "Workspace", "v", "vh", 0);
    if(dataUI->selectedUser != NULL){
        User *user = dataUI->selectedUser;
        Input(workspaceUsers, user->name);
        Input(workspaceUsers, user->surname);
        Input(workspaceUsers, user->pesel);
    }
    
    return workspaceUsers;
}

GtkWidget *WorkspaceBooks(GtkWidget *parent) {
    GtkWidget *workspaceBooks = Div(parent, "Workspace", "v", "vh", 0);
    if(dataUI->selectedBook != NULL){
        Book *book = dataUI->selectedBook;
        Input(workspaceBooks, book->name);
        Input(workspaceBooks, book->author);
    }
    
    return workspaceBooks;
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