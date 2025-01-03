#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"

#include "../data_types/data_types.h"
#include "../iniclialize.h"

void on_YourProjectsListItem_click(GtkGestureClick *gesture, int n_press, double x, double y, char *name) {
	GtkWidget *child = gtk_widget_get_first_child(root);
  while (child != NULL) {
		GtkWidget *next = gtk_widget_get_next_sibling(child);
		gtk_box_remove(root, child);
		child = next;
  }
	data_init(name);
	layoutWorkspace(root);
}

GtkWidget *YourProjectsListItem(GtkWidget *parent, Project *project) {
	GtkWidget *yourProjectsList = Div(parent, "YourProjectsList", "h", "", 4);

	GtkGestureClick *click_gesture = gtk_gesture_click_new();
	g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_YourProjectsListItem_click), project->name);
	gtk_widget_add_controller(yourProjectsList, GTK_EVENT_CONTROLLER(click_gesture));

	GtkWidget *nameTxt = Text(yourProjectsList, "YourProjectsListItemName", project->name, 0);
	gtk_widget_set_hexpand(nameTxt, true);
	Text(yourProjectsList, "YourProjectsListItemDate", project->date, 0);
	Image(yourProjectsList, "Select","play.svg");
}

GtkWidget *YourProjectsList(GtkWidget *parent) {
	GtkWidget *yourProjectsList = Div(parent, "YourProjectsList", "v", "vh", 4);

	FILE *file = fopen("./app/saves/saves.csv", "r");
	char line[512];
	char *name;
	char *date;

	if(file == NULL) { perror("Nie można otworzyć pliku"); return; }

	while(fgets(line, sizeof(line), file) != NULL) {
		line[strcspn(line, "\n")] = '\0';

		name = strtok(line, ",");
		date = strtok(NULL, ",");

		Project *project = projects_add(name, date);
		
		YourProjectsListItem(yourProjectsList, project);
	}
}

GtkWidget *Welcome(GtkWidget *parent) {
  GtkWidget *welcome = Div(parent, "Welcome", "h", "v", 32);

	GtkWidget *leftWelcome = Div(welcome, "LeftWelcome", "v", "vh", 0);
	GtkWidget *welcomeLogo = Div(leftWelcome, "WelcomeLogo", "h", "", 0);
  Text(welcomeLogo, "LogoB", "B", 0);
	Text(welcomeLogo, "LogoRest", "iblioteka", 0);

	Div(leftWelcome, "Space", "h", "vh", 0);

	GtkWidget *buttons = Div(leftWelcome, "WelcomeButtons", "h", "", 0);
	GtkWidget *newLibraryBtn = Div(buttons, "NewLibraryBtn", "v", "", 0);
	Text(newLibraryBtn, "LoadLibraryLabel", "Nowa bibioteka", 0);
	Div(buttons, "Space", "v", "vh", 0);
	GtkWidget *removeLibraryBtn = Div(buttons, "LoadLibraryBtn", "v", "", 0);
	Text(removeLibraryBtn, "LoadLibraryLabel", "Wczytaj z pliku", 0);

	GtkWidget *rightWelcome = Div(welcome, "RightWelcome", "v", "vh", 8);
	Text(rightWelcome, "YourProjectsLabel", "Twoje projekty", 0);
	YourProjectsList(rightWelcome);

  return welcome;
}