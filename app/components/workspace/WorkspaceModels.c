#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"
// "./app/saves/MovieLens_s_dataset_50/NeuMF/metrics_vs_k.png")
GtkWidget *Img(GtkWidget *parent, char *model_name, char *name) {
  GtkWidget *image;

  char src[500];
  strcpy(src, "./app/saves/");
  strcat(src, dataUI->projectName);
  strcat(src, "/");
  strcat(src, model_name);
  strcat(src, "/");
  strcat(src, name);

  image = gtk_image_new_from_file(src);
  gtk_widget_set_name(image, "Img");
  gtk_widget_set_vexpand(image, TRUE); 
  gtk_widget_set_hexpand(image, TRUE); 
  gtk_widget_set_halign(image, GTK_ALIGN_START); 
  gtk_widget_set_valign(image, GTK_ALIGN_START);

  gtk_box_append(parent, image);

  return image;
}
void on_TrainButton_click(GtkGestureClick *gesture, int n_press, double x, double y, User *xz) {
  Model *model = dataUI->selectedModel;

  char buffer[512];
  char resultBuffer[512] = ""; 
  char command[512];
  snprintf(command, sizeof(command), "python3 ML/train_model.py %s %s", dataUI->projectName, model->name);

  FILE *fp;
  fp = popen(command, "r");
  if(fp == NULL) perror("popen");

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf(buffer);
  }
  close(fp);

  printf("Traning finished\n");

  Workspace_rerender();
}

GtkWidget *TrainButton(GtkWidget *parent){
  GtkWidget *trainButton = Div(parent, "TrainButton", "h", "", 4);

  GtkGesture *click = gtk_gesture_click_new();
  g_signal_connect(click, "pressed", G_CALLBACK(on_TrainButton_click), NULL);
  gtk_widget_add_controller(trainButton, GTK_EVENT_CONTROLLER(click));

  Text(trainButton, "TrainButton", "Trenuj model", 0);
}
GtkWidget *WorkspaceModels(GtkWidget *parent) {
  GtkWidget *workspaceModels = Div(parent, "Workspace", "v", "vh", 32);

  GtkWidget *test = Div(workspaceModels, "", "v", "vh", 0);
  if(dataUI->selectedModel == NULL) return workspaceModels;

  GtkWidget *scrollWrapper = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWrapper), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  GtkWidget *contentWrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
  gtk_widget_set_vexpand(contentWrapper, TRUE); 
  gtk_scrolled_window_set_child(scrollWrapper, contentWrapper);
  gtk_box_append(test, scrollWrapper);

  Model *model = dataUI->selectedModel;
  
  GtkWidget *hero = Div(contentWrapper, "Hero", "v", "", 0);
  GtkWidget *nameSurname = Div(hero, "NameSurname", "h", "", 12);
  Text(nameSurname, "ModelName", model->name, 0);

  GtkWidget *desc = Div(contentWrapper, "Desctription", "v", "", 0);
  TrainButton(desc);

  GtkWidget *metrics = Div(contentWrapper, "Metrics", "v", "vh", 0);
  Text(metrics, "MetricsVsEpochsLabel", "Metryki vs Epoki", 0); 
  Img(metrics, model->name, "metrics_vs_epochs.png");

  GtkWidget *metrics_at_k = Div(contentWrapper, "Metrics", "v", "vh", 0);
  Text(metrics_at_k, "MetricsVsEpochsLabel", "Metryki vs Metryki @10", 0);
  Img(metrics_at_k, model->name, "metrics_vs_k.png");

  return workspaceModels;
}