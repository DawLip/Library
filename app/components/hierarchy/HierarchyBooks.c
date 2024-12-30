#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

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

GtkWidget *HierarchyBooks(GtkWidget *parent){
  GtkWidget *hierarchyBooks = Div(parent, "Hierarchy", "v", "", 0);

  Text(hierarchyBooks, "Header", "Books", 0);

  Book *curr = books;
  while(curr != NULL) {
    HierarchyBooksItem(hierarchyBooks, curr);
    curr = curr->next;
  }

  return hierarchyBooks;
}