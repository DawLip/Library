#include <gtk/gtk.h>
#include "./primitives.h"

GtkWidget *Text(GtkWidget *parent, char* name, char* txt, float align){
  GtkWidget *text = gtk_label_new(txt);
  gtk_widget_set_name(text, name);
  gtk_box_append(GTK_BOX(parent), text);

  gtk_label_set_xalign(GTK_LABEL(text), align);
  
  return text;
}