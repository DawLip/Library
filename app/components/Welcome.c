#include <gtk/gtk.h>
// #include <time.h>
// #include <stdio.h>
// #include <sys/stat.h>
// #include <sys/types.h>

#include "./components.h"
#include "../primitives/primitives.h"

#include "../data_types/data_types.h"
#include "../iniclialize.h"

GtkWidget *newLibraryEntry;

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

void on_NewLibraryBtn_click(int *_) {
	time_t now = time(NULL);
	struct tm *localTime = localtime(&now);
  char timeStr[80];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S %Y.%m.%d", localTime);

	char *name = gtk_editable_get_text(newLibraryEntry);
	printf("%s\n", name);

	FILE *file = fopen("./app/saves/saves.csv", "a");
	fprintf(file, "%s,%s\n", name, timeStr);
	fclose(file);

	char pathFile[100];

	char path[100] = "./app/saves/";
	strcat(path, name);
	mkdir(path, 0777);

	strcat(pathFile, path);
	strcat(pathFile, "/users.csv");
	FILE *usersFile = fopen(pathFile, "w+");
	fclose(usersFile);

	strcpy(pathFile,"");
	strcat(pathFile, path);
	strcat(pathFile, "/books.csv");
	FILE *booksFile = fopen(pathFile, "w+");
	fclose(booksFile);

	strcpy(pathFile,"");
	strcat(pathFile, path);
	strcat(pathFile, "/borrowed_books.csv");
	FILE *borrowed_booksFile = fopen(pathFile, "w+");
	fclose(borrowed_booksFile);

	on_YourProjectsListItem_click(NULL, 0, 0, 0, name);
}

GtkWidget *NewLibraryBtn(GtkWidget *parent) {
	GtkWidget *newLibraryBtn = Div(parent, "NewLibraryBtn", "h", "", 0);
	Text(newLibraryBtn, "LoadLibraryLabel", "Nowa bibioteka", 0);

	GtkGestureClick *click_gesture = gtk_gesture_click_new();
	g_signal_connect(click_gesture, "pressed", on_NewLibraryBtn_click, NULL);
	gtk_widget_add_controller(newLibraryBtn, GTK_EVENT_CONTROLLER(click_gesture));
}

GtkWidget *Welcome(GtkWidget *parent) {
  GtkWidget *welcome = Div(parent, "Welcome", "h", "v", 32);

	GtkWidget *leftWelcome = Div(welcome, "LeftWelcome", "v", "vh", 0);
	GtkWidget *welcomeLogo = Div(leftWelcome, "WelcomeLogo", "h", "", 0);
  Text(welcomeLogo, "LogoB", "B", 0);
	Text(welcomeLogo, "LogoRest", "iblioteka", 0);

	Div(leftWelcome, "Space", "h", "vh", 0);

	GtkWidget *buttons = Div(leftWelcome, "WelcomeButtons", "h", "", 0);

	newLibraryEntry = gtk_entry_new();
  gtk_entry_set_placeholder_text(newLibraryEntry, "Nazwa biblioteki...");
  gtk_widget_set_name(newLibraryEntry, "NewLibraryInput");
  gtk_box_append(buttons, newLibraryEntry);

	NewLibraryBtn(buttons);

	GtkWidget *rightWelcome = Div(welcome, "RightWelcome", "v", "vh", 8);
	Text(rightWelcome, "YourProjectsLabel", "Twoje projekty", 0);
	YourProjectsList(rightWelcome);

  return welcome;
}