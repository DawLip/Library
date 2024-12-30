#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *hierarchyWrapper;
GtkWidget *hierarchy;
GtkWidget *parent;

void Hierarchy_render(){
  if(dataUI->currWindow == USERS) hierarchy=HierarchyUsers(hierarchyWrapper);
  else if(dataUI->currWindow == BOOKS) hierarchy=HierarchyBooks(hierarchyWrapper);
}

void Hierarchy_rerender(){
  gtk_box_remove(hierarchyWrapper, hierarchy);
  
  Hierarchy_render();
}

GtkWidget *Hierarchy(GtkWidget *p){
  if(parent==NULL) parent = p;

  hierarchyWrapper = Div(parent, "HierarchyWrapper", "v", "", 0);
  Hierarchy_render();
  
  return hierarchyWrapper;
}