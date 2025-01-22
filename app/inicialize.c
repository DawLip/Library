#pragma once

#include <stdbool.h>
#include <string.h>

#include <gtk/gtk.h>

#include "./primitives/primitives.h"
#include "./components/components.h"

#include "./data_types/data_types.h"
#include "./iniclialize.h"

GtkWidget *window;
GtkWidget *root;

void data_init(char *libraryName) {
  dataUI_init(libraryName);
  users_load_csv();
  books_load_csv();
  borrowed_books_csv();
  models_load_csv();
}

void css_inicialize(){
  GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "./app/style.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}

void window_inicialize(GtkApplication *app) {
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Libary");

  root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), root);
}