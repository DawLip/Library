#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

void on_HierarchyUsersItem_click(GtkGestureClick *gesture, int n_press, double x, double y, User *user) {
    dataUI_set_selectedUser(user);
}

GtkWidget *HierarchyUsersItem(GtkWidget *parent, User *user){
  GtkWidget *hierarchyItem = Div(parent, "HierarchyItem", "h", "h", 4);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyUsersItem_click), user);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  char user_id[12];
  sprintf(user_id, "%d", user->id);

  Text(hierarchyItem, "HierarchyItemId", user_id, 0);
  Text(hierarchyItem, "HierarchyItemName", user->name, 0);
  Text(hierarchyItem, "HierarchyItemSurname", user->surname, 0);

  return hierarchyItem;
}

void on_UserAddButton_click(GtkGestureClick *gesture, int n_press, double x, double y, User *user) {
    users_add("name", "surname", "pesel", "adress", "email", "phone");

    Hierarchy_rerender();
    Workspace_rerender();
}

GtkWidget *UserAddButton(GtkWidget *parent){
  GtkWidget *userAddButton = Div(parent, "UserAddButton", "h", "", 4);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_UserAddButton_click), NULL);
  gtk_widget_add_controller(userAddButton, GTK_EVENT_CONTROLLER(click));

  Image(userAddButton, "AddButtonIcon", "add.svg");
  Text(userAddButton, "AddButtonLabel", "Dodaj czytelnika", 0);
}

// void on_activate(GtkEntry *entry) {
    
    
//     Hierarchy_rerender();
// }

GtkWidget *UserSearcher(GtkWidget *parent, char *value) {
    GtkWidget *entry = gtk_entry_new();
    
    // g_signal_connect(entry, "activate", G_CALLBACK(on_activate), default);
    gtk_widget_set_name(entry, "Searcher");
    gtk_entry_set_placeholder_text(entry, "Szukaj...");
    gtk_widget_set_hexpand(entry, true);

    gtk_box_append(GTK_BOX(parent), entry);
}

GtkWidget *HierarchyUsers(GtkWidget *parent){
  GtkWidget *hierarchyUsers = Div(parent, "Hierarchy", "v", "v", 0);

  Text(hierarchyUsers, "Header", "Czytelnicy", 0);

  User *curr = users;
  while(curr != NULL) {
    HierarchyUsersItem(hierarchyUsers, curr);
    curr = curr->next;
  }

  UserAddButton(hierarchyUsers);
  Div(hierarchyUsers, "space", "v", "vh", 0);

  GtkWidget *userSearch = Div(hierarchyUsers, "Search", "h", "", 8);
  UserSearcher(userSearch, "Szukaj...");
  Image(userSearch, "SearchIcon", "search.svg");

  return hierarchyUsers;
}