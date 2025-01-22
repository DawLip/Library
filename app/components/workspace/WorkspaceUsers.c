#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *workspaceUsers;
GtkWidget *recomendationList;

GtkWidget *ContactItem(GtkWidget *parent, char *icon, char *value, char *placeholder) {
  GtkWidget *contactItem = Div(parent, "ContactItem", "h", "", 12);
  Image(contactItem, "ContactItemIcon", icon);
  Input(contactItem, value, placeholder, on_Input_change, 16);

  return ContactItem;
}

void recomendations_rerender(char *model){
  GtkWidget *child = gtk_widget_get_first_child(recomendationList);
  while (child != NULL) {
		GtkWidget *next = gtk_widget_get_next_sibling(child);
		gtk_box_remove(recomendationList, child);
		child = next;
  }

  User *user = dataUI->selectedUser;
  recomendations_render(recomendationList, model, *user);
}

static void on_usedModel_changed(GtkComboBox *usedModel, Model *model) {
  const gchar *selected_text = gtk_combo_box_text_get_active_text(usedModel);
  recomendations_rerender(selected_text);
}
void BorrowingItem(GtkWidget *parent, char *id, char *name, char *borrow_date, char *return_date, char *nname) {
  GtkWidget *borrowingItem = Div(parent, nname, "h", "", 4); 

  Text(borrowingItem, "BorrowingsItemText1", id, 0);
  GtkWidget *t = Text(borrowingItem, "BorrowingsItemText2", name, 0);
  gtk_widget_set_hexpand(t, TRUE);
  gtk_widget_set_vexpand(t, TRUE);
  Text(borrowingItem, "BorrowingsItemText3", borrow_date, 0.5);
  Text(borrowingItem, "BorrowingsItemText4", return_date, 0.5);
}

void recomendation(GtkWidget *parent, int id){
  Book *book = books;
  while(book->id!=id) {
    if(book->next==NULL) {
      printf("Book not found\n");
      return;
      }
    book = book->next;
  }

  char b_id[10];
  sprintf(b_id, "%d", book->id);
  BorrowingItem(parent, b_id, book->name,"", "","BorrowingsItem");
}

void recomendations_render(GtkWidget *parent, char *model_name, User *user) {
  if(strcmp(model_name, "brak") == 0) return Text(parent, "LackOfRecomendation", "Brak rekomendacji", 0);

  Model *model = models;
  while(strcmp(model->name, model_name)!=0) {
    if(model->next==NULL) {
      printf("Model not found\n");
      return;
      }
    model = model->next;
  }

  Text(parent, "ModelDesctription", model->decription, 0);

  char buffer[512];
  char resultBuffer[512] = ""; 
  char command[512];
  snprintf(command, sizeof(command), "python3 ML/predict.py %s %s %d", dataUI->projectName, model->name, user->id);

  FILE *fp;
  fp = popen(command, "r");
  if(fp == NULL) perror("popen");

  int capture = 0;
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf(buffer);
    if(capture) strncat(resultBuffer, buffer, sizeof(resultBuffer) - strlen(resultBuffer) - 1);
    else if(strncmp(buffer, "RETURN:\n", 8) == 0) capture = 1;
  }

  int item_ids[5];
  int count = 0;
  char *token = strtok(resultBuffer, " ");
  while(token != NULL && count < 5) {
    item_ids[count++] = atoi(token);
    token = strtok(NULL, " ");
  }

  for (int i = 0; i < count; i++) recomendation(parent, item_ids[i]);
  
  pclose(fp);
}

GtkWidget *WorkspaceUsers(GtkWidget *parent) {
  GtkWidget *workspaceUsers = Div(parent, "Workspace", "v", "vh", 0);
  GtkWidget *test = Div(workspaceUsers, "", "v", "vh", 0);

  if(dataUI->selectedUser == NULL) return workspaceUsers;

  GtkWidget *scrollWrapper = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWrapper), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  GtkWidget *contentWrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
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
  BorrowingItem(borrowings, "ID", "Tytuł", "Data wyporzyczenia", "Data oddania", "BorrowingsItemHeader");
  Borrowed_books *curr = borrowed_books;
  while(curr!= NULL) {
    if(curr->user_id != user->id) {curr = curr->next; continue;}  

    Book *book = books;
    while(book->id != curr->book_id ) {
      book = book->next;
    }
    char b_id[10];
    sprintf(b_id, "%d", book->id);
    BorrowingItem(borrowings, b_id, book->name, curr->borrow_date, curr->return_date, "BorrowingsItem");
    curr = curr->next;
  }

  GtkWidget *recomendationListWrapper = Div(contentWrapper, "RecomendationListWrapper", "v", "", 4);
  Text(recomendationListWrapper, "Header", "Rekomendacje:", 0);

  GtkWidget *modelChooser = Div(recomendationListWrapper, "ModelChooserWrapper", "h", "", 8);
  Text(modelChooser, "ModelLabel", "Model:", 0);
  GtkWidget *usedModelCombobox = gtk_combo_box_text_new();
  gtk_combo_box_text_append(usedModelCombobox, NULL, "brak");

  Model *currm = models;
  while(currm != NULL) {
    gtk_combo_box_text_append(usedModelCombobox, NULL, currm->name);
    currm = currm->next;
  }
  gtk_combo_box_set_active(usedModelCombobox, 0);
  gtk_widget_set_name(usedModelCombobox, "ModelChooser");
  g_signal_connect(usedModelCombobox, "changed", G_CALLBACK(on_usedModel_changed), NULL);
  gtk_box_append(modelChooser, usedModelCombobox);

  Text(recomendationListWrapper, "ModelDesctriptionLabel", "Opis modelu:", 0);

  recomendationList = Div(recomendationListWrapper, "RecomendationList", "v", "", 4);
  recomendations_render(recomendationList, "brak", user);

  return workspaceUsers;
}