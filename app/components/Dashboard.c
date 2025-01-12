#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "./components.h"
#include "./../primitives/primitives.h"
#include "./../data_types/data_types.h"

GtkWidget *Dashboard(GtkWidget *parent) {
  GtkWidget *workspaceBooks = Div(parent, "Workspace", "v", "vh", 32);
  if(dataUI->selectedBook == NULL) return workspaceBooks;

  Book *book = dataUI->selectedBook;
  char book_id[15];
  sprintf(book_id, "%d", book->id);

  GtkWidget *hero = Div(workspaceBooks, "Hero", "v", "", 0);
  GtkWidget *nameSurname = Div(hero, "NameSurname", "h", "", 12);
  Input(nameSurname, book->name, "Nazwa", on_Input_change, 48);

  GtkWidget *idWrapper = Div(hero, "IddWrapper", "h", "", 8);
  GtkWidget *id1Wrapper = Div(idWrapper, "Id1Wrapper", "h", "", 4);
  Text(id1Wrapper, "IdLabel", "ID:", 0);
  Text(id1Wrapper, "Id", book_id, 0);
  GtkWidget *id2Wrapper = Div(idWrapper, "Id2Wrapper", "h", "", 4);
  Text(id2Wrapper, "IdLabel", "ISBN:", 0);
  Input(id2Wrapper, book->isbn, "ISBN", on_Input_change, 6);

  GtkWidget *properties = Div(workspaceBooks, "Properties", "v", "", 4);
  LabelValue(properties, "Autor:", book->author, "Autor");
  LabelValue(properties, "Rok wydania:", book->year, "Rok");

  GtkWidget *borrowings = Div(workspaceBooks, "Borrowings", "v", "", 4);
  Text(borrowings, "Header", "Wypożyczenia:", 0);
  Borrowed_books *curr = borrowed_books;
  while(curr!= NULL) {
    if(curr->book_id != book->id) {curr = curr->next; continue;}

    GtkWidget *borrowingItem = Div(borrowings, "BorrowingsItem", "h", "", 4);    

    User *user = users;
    while(user->id != curr->user_id ) {user = user->next;}

    char u_id[10];
    char name_surname[150];
    sprintf(u_id, "%d", user->id);
    sprintf(name_surname, "%s %s", user->name, user->surname);

    Text(borrowingItem, "BorrowingsItemText1", u_id, 0);
    GtkWidget *t = Text(borrowingItem, "BorrowingsItemText2", name_surname, 0);
    gtk_widget_set_hexpand(t, TRUE);
    gtk_widget_set_vexpand(t, TRUE);
    Text(borrowingItem, "BorrowingsItemText3", curr->borrow_date, 0);
    Text(borrowingItem, "BorrowingsItemText4", curr->return_date, 0);

    curr = curr->next;
  }
  
  return workspaceBooks;
}