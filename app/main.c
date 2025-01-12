#include <stdbool.h>
#include <string.h>

#include <gtk/gtk.h>

#include "./primitives/primitives.h"
#include "./components/components.h"

#include "./data_types/data_types.h"
#include "./iniclialize.h"

void layoutWorkspace(GtkWidget *root){
  gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);

  TopBar(root);

  GtkWidget *wrapper = Div(root, "Wrapper", "h", "hv", 0);
  Menu(wrapper);
  Hierarchy(wrapper);
  Workspace(wrapper);
}
void layoutWelcome(GtkWidget *root){
  gtk_window_set_default_size(GTK_WINDOW(window), 384*2, 384);
  Welcome(root);
}

static void activate(GtkApplication *app) {
  window_inicialize(app);
  layoutWelcome(root);
  css_inicialize();

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv){
  int status;

  GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}