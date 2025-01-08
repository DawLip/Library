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

GtkWidget *DivS(GtkWidget *parent, char* name, char* orientation, char* expand, int gap){
  GtkWidget *divWrapper = gtk_scrolled_window_new();

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(divWrapper), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_name(divWrapper, name);


  int box_orientation = strchr(orientation, 'v') ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
  bool box_hexpand = strchr(expand, 'h') != NULL;
  bool box_vexpand = strchr(expand, 'v') != NULL;

  GtkWidget *div = gtk_box_new(box_orientation, gap);
  gtk_widget_set_name(div, name);

  gtk_widget_set_hexpand(div, box_hexpand);
  gtk_widget_set_vexpand(div, box_vexpand); 
  
  gtk_scrolled_window_set_child(divWrapper, div);

  
  gtk_box_append(parent, divWrapper);

  return div;
}

GtkWidget *DivSC(GtkWidget *parent, char* name, char* orientation, char* expand, int gap){
  int box_orientation = strchr(orientation, 'v') ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
  bool box_hexpand = strchr(expand, 'h') != NULL;
  bool box_vexpand = strchr(expand, 'v') != NULL;

  GtkWidget *div = gtk_box_new(box_orientation, gap);
  gtk_widget_set_name(div, name);

  gtk_widget_set_hexpand(div, box_hexpand);
  gtk_widget_set_vexpand(div, box_vexpand); 
  
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(parent), div);

  return div;
}