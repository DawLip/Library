#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *workspaceUsers;

GtkWidget *ContactItem(GtkWidget *parent, char *icon, char *value, char *placeholder) {
  GtkWidget *contactItem = Div(parent, "ContactItem", "h", "", 12);
  Image(contactItem, "ContactItemIcon", icon);
  Input(contactItem, value, placeholder, on_Input_change, 16);

  return ContactItem;
}

static void on_usedModel_changed(GtkComboBox *usedModel, gpointer user_data) {
  const gchar *selected_text = gtk_combo_box_text_get_active_text(usedModel);
  if (strcmp(selected_text, "auto")==0) {
    g_print("Selected item: %s\n", selected_text);
  }
}

void recomendations_render(GtkWidget *parent, User *user) {
  Text(parent, "Header", "Rekomendacje:", 0);

  GtkWidget *usedModelCombobox = gtk_combo_box_text_new();
  gtk_combo_box_text_append(usedModelCombobox, NULL, "auto");
  gtk_combo_box_text_append(usedModelCombobox, NULL, "NeuMF simple");
  gtk_combo_box_text_append(usedModelCombobox, NULL, "NeuMF advanced");
  gtk_combo_box_text_append(usedModelCombobox, NULL, "MostBorrows");
  gtk_combo_box_set_active(usedModelCombobox, 0);
  g_signal_connect(usedModelCombobox, "changed", G_CALLBACK(on_usedModel_changed), NULL);
  gtk_box_append(parent, usedModelCombobox);

  char buffer[512];
  FILE *fp;
  char command[512];
  snprintf(command, sizeof(command), "python3 python_functions/predict.py %s %d %s", dataUI->projectName, user->id);
  fp = popen(command, "r");
  if(fp == NULL) perror("popen");

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf("%s", buffer);
  }
  pclose(fp);
}

GtkWidget *WorkspaceUsers(GtkWidget *parent) {
  GtkWidget *workspaceUsers = Div(parent, "Workspace", "v", "vh", 0);
  GtkWidget *test = Div(workspaceUsers, "", "v", "vh", 0);
  if(dataUI->selectedUser == NULL) return workspaceUsers;

  GtkWidget *scrollWrapper = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWrapper), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);


  GtkWidget *contentWrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // gtk_widget_set_hexpand(contentWrapper, box_hexpand);
  gtk_widget_set_vexpand(contentWrapper, TRUE); 
  
  gtk_scrolled_window_set_child(scrollWrapper, contentWrapper);

  
  gtk_box_append(test, scrollWrapper);


  User *user = dataUI->selectedUser;
  char user_id[10];
  sprintf(user_id, "%d", user->id);

  GtkWidget *hero = Div(contentWrapper, "Hero", "v", "", 0);
  GtkWidget *nameSurname = Div(hero, "NameSurname", "h", "", 12);
  Input(nameSurname, user->name, "Imię", on_Input_change, 48);
  Input(nameSurname, user->surname, "Naz", on_Input_change, 48);

  GtkWidget *idWrapper = Div(hero, "IddWrapper", "h", "", 8);
  GtkWidget *id1Wrapper = Div(idWrapper, "Id1Wrapper", "h", "", 4);
  Text(id1Wrapper, "IdLabel", "ID:", 0);
  Text(id1Wrapper, "Id", user_id, 0);
  GtkWidget *id2Wrapper = Div(idWrapper, "Id2Wrapper", "h", "", 4);
  Text(id2Wrapper, "IdLabel", "Pesel:", 0);
  Input(id2Wrapper, user->pesel, "Pesel", on_Input_change, 6);

  GtkWidget *contact = Div(contentWrapper, "Contact", "v", "", 4);
  Text(contact, "Header", "Kontakt:", 0);
  ContactItem(contact, "email.svg", user->email, "Email");
  ContactItem(contact, "phone.svg", user->phone, "Telefon");
  ContactItem(contact, "home.svg", user->adress, "Adres");

  GtkWidget *borrowings = Div(contentWrapper, "Borrowings", "v", "", 4);
  Text(borrowings, "Header", "Wypożyczenia:", 0);
  Borrowed_books *curr = borrowed_books;
  while(curr!= NULL) {
    if(curr->user_id != user->id) {curr = curr->next; continue;}

    GtkWidget *borrowingItem = Div(borrowings, "BorrowingsItem", "h", "", 4);    

    Book *book = books;
    while(book->id != curr->book_id ) {
      book = book->next;
    }
    Text(borrowingItem, "BorrowingsItemText", book->name, 0);
    Text(borrowingItem, "BorrowingsItemText", curr->borrow_date, 0);
    Text(borrowingItem, "BorrowingsItemText", curr->return_date, 0);
    curr = curr->next;
  }

  GtkWidget *recomendationList = Div(contentWrapper, "RecomendationList", "v", "", 4);
  recomendations_render(recomendationList, user);

  return workspaceUsers;
}