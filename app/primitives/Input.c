#include <gtk/gtk.h>
#include "./primitives.h"

GtkWidget *Input(GtkWidget *parent, char* value, char* placeholder, void (*func)(), int size) {
  GtkWidget *entry = gtk_entry_new();
  
  g_signal_connect(entry, "changed", G_CALLBACK(func), value);

  gtk_editable_set_text(entry, value);
  gtk_entry_set_placeholder_text(entry, placeholder);

  gtk_widget_set_hexpand(entry, TRUE);
  gtk_widget_set_vexpand(entry, TRUE); 

  gtk_widget_set_name(entry, "input");

  gtk_box_append(GTK_BOX(parent), entry);
}