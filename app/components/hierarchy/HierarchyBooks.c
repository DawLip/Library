#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

void on_BookAddButton_click(GtkGestureClick *gesture, int n_press, double x, double y, Book *book) {
    int id=0;
    if(books!=NULL){
      Book *curr=books;
      while(curr->next!=NULL) curr = curr->next;
      id=curr->id+1;
    }
    books_add(id,"name", "author", "isbn", "9999", 1);

    Hierarchy_rerender();
    Workspace_rerender();
}

GtkWidget *BookAddButton(GtkWidget *parent){
  GtkWidget *bookAddButton = Div(parent, "BookAddButton", "h", "", 4);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_BookAddButton_click), NULL);
  gtk_widget_add_controller(bookAddButton, GTK_EVENT_CONTROLLER(click));

  Image(bookAddButton, "AddButtonIcon", "add.svg");
  Text(bookAddButton, "AddButtonLabel", "Dodaj Książkę", 0);
}

void on_HierarchyBookItem_click(GtkGestureClick *gesture, int n_press, double x, double y, Book *book) {
    dataUI_set_selectedBook(book);
}

void on_HierarchyBooksItem_click(Book *book) {
  dataUI_set_selectedBook(book);
}

GtkWidget *HierarchyBooksItem(GtkWidget *parent, Book *book){
  User *selected_book = dataUI->selectedBook;
  GtkWidget *hierarchyItem;
  if(dataUI->selectedBook!=NULL && selected_book->id==book->id) 
    hierarchyItem= Div(parent, "HierarchyItemSelected", "h", "h", 4);
  else 
    hierarchyItem = Div(parent, "HierarchyItem", "h", "h", 4);

  char book_id[15];
  sprintf(book_id, "%d", book->id);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyBookItem_click), book);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItem", book_id, 0);
  Text(hierarchyItem, "HierarchyItem", book->name, 0);

  return hierarchyItem;
}

GtkWidget *BookSearcher(GtkWidget *parent, char *value) {
    GtkWidget *entry = gtk_entry_new();
    
    gtk_widget_set_name(entry, "Searcher");
    gtk_entry_set_placeholder_text(entry, "Szukaj...");
    gtk_widget_set_hexpand(entry, true);

    gtk_box_append(GTK_BOX(parent), entry);
}

int hierarchyBooksItemRenderCondition(Book *curr) {
  const char *text = NULL;
  if(hierarchySearcher!=NULL) text = gtk_editable_get_text(hierarchySearcher);

  if(text==NULL || strcmp(text, "")==0) return 1;

  char curr_id[15];
  sprintf(curr_id, "%d", curr->id);

  char str[100];
  strcpy(str, text);
  char *token = strtok(str, " ");

  while (token != NULL) {
    if(
      strstr(curr_id, token)!=NULL || 
      strstr(curr->name, token)!=NULL || 
      strstr(curr->author, token)!=NULL
    ) return 1;
    token = strtok(NULL, " "); 
  }

  return 0;
}

GtkWidget *HierarchyBooks(GtkWidget *parent){
  GtkWidget *hierarchyBooks = Div(parent, "Hierarchy", "v", "vh", 0);  

  Text(hierarchyBooks, "Header", "Books", 0);

  GtkWidget *itemList = DivS(hierarchyBooks, "ItemList", "v", "vh", 0);
  Book *curr = books;
  while(curr != NULL) {
    if(hierarchyBooksItemRenderCondition(curr)) HierarchyBooksItem(itemList, curr);
    curr = curr->next;
  }

  BookAddButton(hierarchyBooks);

  return hierarchyBooks;
}