#include <gtk/gtk.h>
#include "./primitives.h"

static void on_entry_changed(GtkEntry *entry, int size) {
    const char *text = gtk_editable_get_text(entry);

    int width = strlen(text) * size; 
    gtk_widget_set_size_request(GTK_WIDGET(entry), width, 10);
}

GtkWidget *Input(GtkWidget *parent, char* value, char* placeholder, void (*func)(), int size) {
    GtkWidget *entry = gtk_entry_new();
    
    g_signal_connect(entry, "activate", G_CALLBACK(func), value);
		g_signal_connect(entry, "changed", G_CALLBACK(on_entry_changed), size);

    gtk_editable_set_text(entry, value);
		gtk_entry_set_placeholder_text(entry, placeholder);
		// gtk_widget_set_size_request(entry, -1, 10);

		gtk_widget_set_name(entry, "input");

    gtk_box_append(GTK_BOX(parent), entry);
}