#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"

GtkWidget *projectName;

void item_remove(GtkWidget *button, gpointer user_data) {
	if(dataUI->currWindow==USERS) users_remove(button, user_data);
	else if(dataUI->currWindow==BOOKS) books_remove(button, user_data);
}

GtkWidget *TopBar(GtkWidget *parent) {
  GtkWidget *topBar = Div(parent, "TopBar", "h", "", 0);

	GtkWidget *logoWrapper = Div(topBar, "LogoWrapper", "v", "", 0);
  Text(logoWrapper, "Logo", "B", 0.5);

	GtkWidget *projectNameAndSave = Div(topBar, "ProjectNameAndSave", "h","", 0);
	projectName = Text(projectNameAndSave, "ProjectName", "Moja biblioteka", 0);
	gtk_widget_set_hexpand(projectName, TRUE);
	Image(projectNameAndSave, "SaveProject", "save.svg");

	GtkWidget *topBarOptions = Div(topBar, "TopBarOptions", "h","h", 16);
	// Text(topBarOptions, "Edit", "Edytuj", 0);
	GtkWidget *removeBookBtn = Text(topBarOptions, "Remove", "Usuń", 0);
	GtkGestureClick *click_gesture = gtk_gesture_click_new();
	g_signal_connect(click_gesture, "pressed", G_CALLBACK(item_remove), NULL);
	gtk_widget_add_controller(removeBookBtn, GTK_EVENT_CONTROLLER(click_gesture));

  return topBar;
}