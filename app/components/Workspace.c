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

GtkWidget *ContactItem(GtkWidget *parent, char *icon, char *value, char *placeholder) {
    GtkWidget *contactItem = Div(parent, "ContactItem", "h", "", 12);
    Image(contactItem, "ContactItemIcon", icon);
    Input(contactItem, value, placeholder, on_activate, 16);

    return ContactItem;
}

GtkWidget *LabelValue(GtkWidget *parent, char *label, char *value, char *placeholder) {
    GtkWidget *labelValue = Div(parent, "LabelValue", "h", "", 12);
    Text(labelValue, "LabelValueLabel", label, 0);
    Input(labelValue, value, placeholder, on_activate, 16);

    return labelValue;
}

GtkWidget *WorkspaceUsers(GtkWidget *parent) {
    GtkWidget *workspaceUsers = Div(parent, "Workspace", "v", "vh", 64);
    if(dataUI->selectedUser == NULL) return workspaceUsers;

    User *user = dataUI->selectedUser;
    char user_id[10];
    sprintf(user_id, "%d", user->id);

    GtkWidget *hero = Div(workspaceUsers, "Hero", "v", "", 0);
    GtkWidget *nameSurname = Div(hero, "NameSurname", "h", "", 12);
    Input(nameSurname, user->name, "Imię", on_activate, 48);
    Input(nameSurname, user->surname, "Naz", on_activate, 48);

    GtkWidget *idWrapper = Div(hero, "IddWrapper", "h", "", 8);
    GtkWidget *id1Wrapper = Div(idWrapper, "Id1Wrapper", "h", "", 4);
    Text(id1Wrapper, "IdLabel", "ID:", 0);
    Text(id1Wrapper, "Id", user_id, 0);
    GtkWidget *id2Wrapper = Div(idWrapper, "Id2Wrapper", "h", "", 4);
    Text(id2Wrapper, "IdLabel", "Pesel:", 0);
    Input(id2Wrapper, user->pesel, "Pesel", on_activate, 6);

    GtkWidget *contact = Div(workspaceUsers, "Contact", "v", "", 4);
    Text(contact, "ContactHeader", "Kontakt:", 0);
    ContactItem(contact, "email.svg", user->email, "Email");
    ContactItem(contact, "phone.svg", user->phone, "Telefon");
    ContactItem(contact, "home.svg", user->adress, "Adres");

    return workspaceUsers;
}

GtkWidget *WorkspaceBooks(GtkWidget *parent) {
    GtkWidget *workspaceBooks = Div(parent, "Workspace", "v", "vh", 32);
    if(dataUI->selectedBook == NULL) return workspaceBooks;

    Book *book = dataUI->selectedBook;
    char book_id[15];
    sprintf(book_id, "%d", book->id);

    GtkWidget *hero = Div(workspaceBooks, "Hero", "v", "", 0);
    GtkWidget *nameSurname = Div(hero, "NameSurname", "h", "", 12);
    Input(nameSurname, book->name, "Nazwa", on_activate, 48);

    GtkWidget *idWrapper = Div(hero, "IddWrapper", "h", "", 8);
    GtkWidget *id1Wrapper = Div(idWrapper, "Id1Wrapper", "h", "", 4);
    Text(id1Wrapper, "IdLabel", "ID:", 0);
    Text(id1Wrapper, "Id", book_id, 0);
    GtkWidget *id2Wrapper = Div(idWrapper, "Id2Wrapper", "h", "", 4);
    Text(id2Wrapper, "IdLabel", "ISBN:", 0);
    Input(id2Wrapper, book->isbn, "ISBN", on_activate, 6);

    GtkWidget *properties = Div(workspaceBooks, "Properties", "v", "", 4);
    LabelValue(properties, "Autor:", book->author, "Autor");
    LabelValue(properties, "Rok wydania:", book->year, "Rok");
    
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