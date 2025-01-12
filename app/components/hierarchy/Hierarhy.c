#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

GtkWidget *hierarchyWrapper;
GtkWidget *hierarchyContainer;
GtkWidget *hierarchy;
GtkWidget *hierarchyParent;

GtkWidget *hierarchySearcher;

static void on_HierarchySearch_changed(GtkEntry *entry, int size) {
    Hierarchy_rerender();
}

GtkWidget *Searcher(GtkWidget *parent, char *value) {
    hierarchySearcher = gtk_entry_new();
    
    gtk_widget_set_name(hierarchySearcher, "Searcher");
    gtk_entry_set_placeholder_text(hierarchySearcher, "Szukaj...");
    gtk_widget_set_hexpand(hierarchySearcher, true);

    g_signal_connect(hierarchySearcher, "changed", G_CALLBACK(on_HierarchySearch_changed), 0);

    gtk_box_append(GTK_BOX(parent), hierarchySearcher);
}

void Hierarchy_render(){
  if(dataUI->currWindow == USERS) hierarchy=HierarchyUsers(hierarchyContainer);
  else if(dataUI->currWindow == BOOKS) hierarchy=HierarchyBooks(hierarchyContainer);
  else if(dataUI->currWindow == DASHBOARD) hierarchy=HierarchyDashboard(hierarchyContainer);
}

void Hierarchy_rerender(){
  gtk_box_remove(hierarchyContainer, hierarchy);
  
  Hierarchy_render();
}

GtkWidget *Hierarchy(GtkWidget *p){
  if(hierarchyParent==NULL) hierarchyParent = p;

  hierarchyWrapper = Div(hierarchyParent, "HierarchyWrapper", "v", "v", 0);
  hierarchyContainer = Div(hierarchyWrapper, "HierarchyContainer", "v", "v", 0);
  Hierarchy_render();

  GtkWidget *search = Div(hierarchyWrapper, "Search", "h", "", 8);
  Searcher(search, "Szukaj...");
  Image(search, "SearchIcon", "search.svg");
  
  return hierarchyWrapper;
}