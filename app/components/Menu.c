#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

GtkWidget *menu;

void on_MenuIcon_click(GtkGestureClick *gesture, int n_press, double x, double y, char *name) {
    if(strstr(name, "books")!=NULL) dataUI_set_currWindow(BOOKS);
    else if(strstr(name, "users")!=NULL) dataUI_set_currWindow(USERS);

    Menu_rerender();
}
// pointer do name przestaje istnieć //TODO 
GtkWidget *MenuIcon(GtkWidget *parent, char *name) {
    GtkWidget *menuIcon = Image(parent, "MenuIcon", name);

    GtkGestureClick *click_gesture = gtk_gesture_click_new();
    g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_MenuIcon_click), name);
    gtk_widget_add_controller(menuIcon, GTK_EVENT_CONTROLLER(click_gesture));

    return menuIcon;
}

GtkWidget *Menu(GtkWidget *parent) {
  menu = Div(parent, "Menu", "v", "", 16);

  Menu_render();
  
  return menu;
}

void Menu_render() {
  if(dataUI->currWindow==DASHBOARD) MenuIcon(menu, "home-active.svg");
  else MenuIcon(menu, "home.svg");

  if(dataUI->currWindow==USERS) MenuIcon(menu, "users-active.svg");
  else MenuIcon(menu, "users.svg");

  if(dataUI->currWindow==BOOKS) MenuIcon(menu, "books-active.svg");
  else MenuIcon(menu, "books.svg");
}

void Menu_rerender(){
  GtkWidget *child = gtk_widget_get_first_child(menu);
  while (child != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(menu, child);
        child = next;
    }
  
  Menu_render();
}