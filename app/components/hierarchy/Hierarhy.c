#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *hierarchyUsers;
GtkWidget *hierarchyBooks;

GtkWidget *Hierarchy(GtkWidget *parent){
  hierarchyUsers=HierarchyUsers(parent);
  hierarchyBooks=HierarchyBooks(parent);

  gtk_widget_hide(hierarchyBooks);
}

void Hierarchy_rerender(){
  if(dataUI->currWindow==USERS){
    gtk_widget_hide(hierarchyBooks);
    gtk_widget_show(hierarchyUsers);
  } else if(dataUI->currWindow==BOOKS){
    gtk_widget_hide(hierarchyUsers);
    gtk_widget_show(hierarchyBooks);
  }
}