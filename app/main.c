#include <stdbool.h>
#include <string.h>

#include <gtk/gtk.h>

#include "./primitives/primitives.h"
#include "./components/components.h"

#include "./data_types/data_types.h"

void layout(GtkWidget *root){
  GtkWidget *wrapper;

  Div(root, "TopBar", "h", "h", 0);

  wrapper = Div(root, "Wrapper", "h", "hv", 0);
  Menu(wrapper);
  Hierarchy(wrapper);
  Workspace(wrapper);

}

void css_inicialize(){
  GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "./app/style.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}

static void activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Libary");
  gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);

  GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), root);
  
  layout(root);
  css_inicialize();

  gtk_window_present(GTK_WINDOW(window));
}

void data_init() {
  users_load_csv();
  books_load_csv();
  dataUI_init();
}

int main(int argc, char **argv){
  GtkApplication *app;
  int status;

  data_init();

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}