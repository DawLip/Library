#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

void on_HierarchyUsersItem_click(GtkGestureClick *gesture, int n_press, double x, double y, User *user) {
    dataUI_set_selectedUser(user);
}

void on_HierarchyBookItem_click(GtkGestureClick *gesture, int n_press, double x, double y, Book *book) {
    dataUI_set_selectedBook(book);
}

GtkWidget *HierarchyUsersItem(GtkWidget *parent, User *user){
  GtkWidget *hierarchyItem = Div(parent, "HierarchyItem", "h", "", 0);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyUsersItem_click), user);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItem", user->name, 0);

  return hierarchyItem;
}

GtkWidget *HierarchyBooksItem(GtkWidget *parent, Book *book){
  GtkWidget *hierarchyItem = Div(parent, "HierarchyItem", "h", "", 0);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyBookItem_click), book);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItem", book->name, 0);

  return hierarchyItem;
}

GtkWidget *HierarchyUsers(GtkWidget *parent){
  GtkWidget *hierarchyUsers = Div(parent, "Hierarchy", "v", "", 0);

  Text(hierarchyUsers, "Header", "Users", 0);

  User *curr = users;
  while(curr != NULL) {
    HierarchyUsersItem(hierarchyUsers, curr);
    curr = curr->next;
  }

  return hierarchyUsers;
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

GtkWidget *hierarchyUsers;
GtkWidget *hierarchyBooks;

GtkWidget *Hierarchy(GtkWidget *parent){
  hierarchyUsers=HierarchyUsers(parent);
  hierarchyBooks=HierarchyBooks(parent);

  gtk_widget_hide(hierarchyBooks);
}

void Hierarchy_rerender(){
  if(dataUI->currWindow==USERS){
    gtk_widget_hide(hierarchyBooks);
    gtk_widget_show(hierarchyUsers);
  } else if(dataUI->currWindow==BOOKS){
    gtk_widget_hide(hierarchyUsers);
    gtk_widget_show(hierarchyBooks);
  }
}