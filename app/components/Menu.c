#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"

void on_MenuIcon_click(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {
    g_print("Obraz został kliknięty! Kliknięcie %d w punkcie (%f, %f)\n", n_press, x, y);
}

GtkWidget *MenuIcon(GtkWidget *parent, char *name) {
    GtkWidget *menuIcon = Image(parent, "MenuIcon", name);

    GtkGestureClick *click_gesture = gtk_gesture_click_new();
    g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_MenuIcon_click), NULL);

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