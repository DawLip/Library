#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *HierarchyDashboard(GtkWidget *parent){
  GtkWidget *hierarchyDashboard = Div(parent, "Hierarchy", "v", "vh", 0);  

  return hierarchyDashboard;
}