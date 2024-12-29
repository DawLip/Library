#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

void on_MenuIcon_click(GtkGestureClick *gesture, int n_press, double x, double y, gpointer name) {
    if(strcmp(name, "books.png")==0)      dataUI_set_currWindow(BOOKS);
    else if(strcmp(name, "users.svg")==0) dataUI_set_currWindow(USERS);
}

GtkWidget *MenuIcon(GtkWidget *parent, char *name) {
    GtkWidget *menuIcon = Image(parent, "MenuIcon", name);

    GtkGestureClick *click_gesture = gtk_gesture_click_new();
    g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_MenuIcon_click), name);
    gtk_widget_add_controller(menuIcon, GTK_EVENT_CONTROLLER(click_gesture));

    return menuIcon;
}

GtkWidget *Menu(GtkWidget *parent) {
  GtkWidget *menu;
  menu = Div(parent, "Menu", "v", "", 16);

  MenuIcon(menu, "home.svg");
  MenuIcon(menu, "users.svg");
  MenuIcon(menu, "books.png");
  
  return menu;
}