#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *LabelValue(GtkWidget *parent, char *label, char *value, char *placeholder) {
  GtkWidget *labelValue = Div(parent, "LabelValue", "h", "", 12);
  Text(labelValue, "LabelValueLabel", label, 0);
  Input(labelValue, value, placeholder, on_Input_change, 16);

  return labelValue;
}
GtkWidget *WorkspaceBooks(GtkWidget *parent) {
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
  Text(borrowings, "BorrowingsHeader", "Wypożyczenia:", 0);
  Borrowed_books *curr = borrowed_books;
  while(curr!= NULL) {
    if(curr->book_id != book->id) {curr = curr->next; continue;}

    GtkWidget *borrowingItem = Div(borrowings, "BorrowingsItem", "h", "", 4);    

    User *user = users;
    while(user->id != curr->user_id ) {user = user->next;}
    Text(borrowingItem, "BorrowingsItemText", user->name, 0);
    Text(borrowingItem, "BorrowingsItemText", user->surname, 0);
    Text(borrowingItem, "BorrowingsItemText", curr->borrow_date, 0);
    Text(borrowingItem, "BorrowingsItemText", curr->return_date, 0);
    curr = curr->next;
  }
  
  return workspaceBooks;
}