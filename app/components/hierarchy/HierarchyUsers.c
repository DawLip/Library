#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

void on_HierarchyUsersItem_click(GtkGestureClick *gesture, int n_press, double x, double y, User *user) {
    dataUI_set_selectedUser(user);
}

GtkWidget *HierarchyUsersItem(GtkWidget *parent, User *user){
  GtkWidget *hierarchyItem = Div(parent, "HierarchyItem", "h", "", 0);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyUsersItem_click), user);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItem", user->name, 0);

  return hierarchyItem;
}

void on_UserAddButton_click(GtkGestureClick *gesture, int n_press, double x, double y, User *user) {
    users_add("name", "surname", "pesel");

    Hierarchy_rerender();
    Workspace_rerender();
}

GtkWidget *UserAddButton(GtkWidget *parent){
  GtkWidget *userAddButton = Div(parent, "UserAddButton", "v", "", 0);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_UserAddButton_click), NULL);
  gtk_widget_add_controller(userAddButton, GTK_EVENT_CONTROLLER(click));

  Text(userAddButton, "UserAddButtonLabel", "Add user", 0);
}

GtkWidget *HierarchyUsers(GtkWidget *parent){
  GtkWidget *hierarchyUsers = Div(parent, "Hierarchy", "v", "", 0);

  Text(hierarchyUsers, "Header", "Users", 0);

  User *curr = users;
  while(curr != NULL) {
    HierarchyUsersItem(hierarchyUsers, curr);
    curr = curr->next;
  }

  UserAddButton(hierarchyUsers);

  return hierarchyUsers;
}