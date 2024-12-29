#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

GtkWidget *WorkspaceName;
GtkWidget *WorkspaceSurname;
GtkWidget *WorkspacePesel;

GtkWidget *Workspace(GtkWidget *parent) {
    GtkWidget *workspace = Div(parent, "Workspace", "v", "vh", 0);

    WorkspaceName = Text(workspace, "WorkspaceName", "", 0);
    WorkspaceSurname = Text(workspace, "WorkspaceSurname", "", 0);
    WorkspacePesel = Text(workspace, "WorkspacePesel", "", 0);
  
    return workspace;
}

void Workspace_rerender() {
    User *user = dataUI->selectedUser;
    
    gtk_label_set_text(WorkspaceName, user->name);
    gtk_label_set_text(WorkspaceSurname, user->surname);
    gtk_label_set_text(WorkspacePesel, user->pesel);
}