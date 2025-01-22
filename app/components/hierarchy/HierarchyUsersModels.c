#include <gtk/gtk.h>
#include <string.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

void on_HierarchyModelsItem_click(GtkGestureClick *gesture, int n_press, double x, double y, Model *model) {
    dataUI_set_selectedModel(model);
}

GtkWidget *HierarchyModelsItem(GtkWidget *parent, Model *model){
  Model *selected_model = dataUI->selectedModel;
  GtkWidget *hierarchyItem;
  if(dataUI->selectedModel!=NULL && strcmp(selected_model->name, model->name)==0) 
    hierarchyItem= Div(parent, "HierarchyItemSelected", "h", "h", 4);
  else 
    hierarchyItem = Div(parent, "HierarchyItem", "h", "h", 4);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_HierarchyModelsItem_click), model);
  gtk_widget_add_controller(hierarchyItem, GTK_EVENT_CONTROLLER(click));

  Text(hierarchyItem, "HierarchyItemId", model->name, 0);;

  return hierarchyItem;
}

GtkWidget *HierarchyModels(GtkWidget *parent){
  GtkWidget *hierarchyModels = Div(parent, "Hierarchy", "v", "vh", 0);
  
  Text(hierarchyModels, "Header", "Modele", 0);

  GtkWidget *itemList = DivS(hierarchyModels, "ItemList", "v", "vh", 0);
  Model *curr = models;
  while(curr != NULL) {
    HierarchyModelsItem(itemList, curr);
    curr = curr->next;
  }

  return hierarchyModels;
}