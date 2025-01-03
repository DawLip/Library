#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

void on_BookAddButton_click(GtkGestureClick *gesture, int n_press, double x, double y, Book *book) {
    books_add("name", "author", "isbn", "9999", 1);

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

GtkWidget *HierarchyBooksItem(GtkWidget *parent, Book *book){
  GtkWidget *hierarchyItem = Div(parent, "HierarchyItem", "h", "", 0);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyBookItem_click), book);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItem", book->name, 0);

  return hierarchyItem;
}

GtkWidget *BookSearcher(GtkWidget *parent, char *value) {
    GtkWidget *entry = gtk_entry_new();
    
    // g_signal_connect(entry, "activate", G_CALLBACK(on_activate), default);
    gtk_widget_set_name(entry, "Searcher");
    gtk_entry_set_placeholder_text(entry, "Szukaj...");
    gtk_widget_set_hexpand(entry, true);

    gtk_box_append(GTK_BOX(parent), entry);
}

GtkWidget *HierarchyBooks(GtkWidget *parent){
  GtkWidget *hierarchyBooks = Div(parent, "Hierarchy", "v", "vh", 0);

  Text(hierarchyBooks, "Header", "Books", 0);

  Book *curr = books;
  while(curr != NULL) {
    HierarchyBooksItem(hierarchyBooks, curr);
    curr = curr->next;
  }

  BookAddButton(hierarchyBooks);
  Div(hierarchyBooks, "space", "h", "vh", 0);

  GtkWidget *bookSearch = Div(hierarchyBooks, "Search", "h", "", 8);
  BookSearcher(bookSearch, "Szukaj...");
  Image(bookSearch, "SearchIcon", "search.svg");

  return hierarchyBooks;
}