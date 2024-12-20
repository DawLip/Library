#include <gtk/gtk.h>
#include "./primitives.h"

GtkWidget *Div(GtkWidget *parent, char* name, char* orientation, char* expand, int gap){
  int box_orientation = strchr(orientation, 'v') ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
  bool box_hexpand = strchr(expand, 'h') != NULL;
  bool box_vexpand = strchr(expand, 'v') != NULL;

  GtkWidget *div = gtk_box_new(box_orientation, gap);
  gtk_widget_set_name(div, name);

  gtk_box_append(GTK_BOX(parent), div);

  gtk_widget_set_hexpand(div, box_hexpand);
  gtk_widget_set_vexpand(div, box_vexpand); 
  
  return div;
}