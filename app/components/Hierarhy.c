#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

void on_HierarchyItem_click(GtkGestureClick *gesture, int n_press, double x, double y, User *user) {
    dataUI_set_selectedUser(user);
}

GtkWidget *HierarchyItem(GtkWidget *parent, User *user){
  GtkWidget *hierarchyItem = Div(parent, "HierarchyItem", "h", "", 0);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyItem_click), user);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItem", user->name, 0);

  return hierarchyItem;
}

GtkWidget *Hierarchy(GtkWidget *parent){
  GtkWidget *hierarchy = Div(parent, "Hierarchy", "v", "", 0);

  Text(hierarchy, "Header", "Users", 0);

  User *curr = users;
  while(curr != NULL) {
    HierarchyItem(hierarchy, curr);
    curr = curr->next;
  }

  return hierarchy;
}