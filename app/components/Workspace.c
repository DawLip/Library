#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

GtkWidget *workspaceUsers;
GtkWidget *WorkspaceUsersName;
GtkWidget *WorkspaceUsersSurname;
GtkWidget *WorkspaceUsersPesel;

GtkWidget *workspaceBooks;
GtkWidget *WorkspaceBooksName;
GtkWidget *WorkspaceBooksAuthor;

GtkWidget *WorkspaceUsers(GtkWidget *parent) {
    GtkWidget *workspaceUsers = Div(parent, "Workspace", "v", "vh", 0);

    WorkspaceUsersName = Text(workspaceUsers, "WorkspaceName", "", 0);
    WorkspaceUsersSurname = Text(workspaceUsers, "WorkspaceSurname", "", 0);
    WorkspaceUsersPesel = Text(workspaceUsers, "WorkspacePesel", "", 0);
  
    return workspaceUsers;
}

GtkWidget *WorkspaceBooks(GtkWidget *parent) {
    GtkWidget *workspaceBooks = Div(parent, "Workspace", "v", "vh", 0);

    WorkspaceBooksName = Text(workspaceBooks, "WorkspaceName", "", 0);
    WorkspaceBooksAuthor = Text(workspaceBooks, "WorkspaceAuthor", "", 0);
  
    return workspaceBooks;
}

GtkWidget *Workspace(GtkWidget *parent) {
    workspaceUsers=WorkspaceUsers(parent);
    workspaceBooks=WorkspaceBooks(parent);

    gtk_widget_hide(workspaceBooks);
}

void Workspace_rerender() {
    if(dataUI->currWindow==USERS){
        gtk_widget_hide(workspaceBooks);
        gtk_widget_show(workspaceUsers);
    } else if(dataUI->currWindow==BOOKS){
        gtk_widget_hide(workspaceUsers);
        gtk_widget_show(workspaceBooks);
    }

    if(dataUI->currWindow==USERS && dataUI->selectedUser != NULL){
        User *user = dataUI->selectedUser;
    
        gtk_label_set_text(WorkspaceUsersName, user->name);
        gtk_label_set_text(WorkspaceUsersSurname, user->surname);
        gtk_label_set_text(WorkspaceUsersPesel, user->pesel);
    } else if(dataUI->currWindow==BOOKS && dataUI->selectedBook){
        Book *book = dataUI->selectedBook;
    
        gtk_label_set_text(WorkspaceBooksName, book->name);
        gtk_label_set_text(WorkspaceBooksAuthor, book->author);
    }
}