#include <gtk/gtk.h>
#include <string.h>

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

int hierarchyUsersItemRenderCondition(User *curr) {
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
      strstr(curr->surname, token)!=NULL
    ) return 1;
    token = strtok(NULL, " "); 
  }

  return 0;
}

GtkWidget *HierarchyUsers(GtkWidget *parent){
  GtkWidget *hierarchyUsers = Div(parent, "Hierarchy", "v", "vh", 0);
  
  Text(hierarchyUsers, "Header", "Czytelnicy", 0);
  User *curr = users;
  while(curr != NULL) {
    if(hierarchyUsersItemRenderCondition(curr)) HierarchyUsersItem(hierarchyUsers, curr);
    curr = curr->next;
  }

  UserAddButton(hierarchyUsers);

  return hierarchyUsers;
}