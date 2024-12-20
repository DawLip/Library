#include <gtk/gtk.h>
#include "./primitives.h"

GtkWidget *Image(GtkWidget *parent, char* name, char* src){
  char SRC[80];
  strcpy(SRC, "./images/");
  strcat(SRC, src);

  GtkWidget *image = gtk_image_new_from_file(SRC);
  gtk_widget_set_name(image, name);
  gtk_box_append(GTK_BOX(parent), image);
  
  return image;
}