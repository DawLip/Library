#include <gtk/gtk.h>

#include "./components.h"
#include "../primitives/primitives.h"
#include "../data_types/data_types.h"
#include "../iniclialize.h"

GtkWidget *projectName;

void item_remove(GtkWidget *button, gpointer user_data) {
	if(dataUI->currWindow==USERS) users_remove(button, user_data);
	else if(dataUI->currWindow==BOOKS) books_remove(button, user_data);
}

void on_SaveIcon_click(GtkWidget *button, gpointer user_data){
	char pathFile[100];
	char path[100] = "./app/saves/";
	strcat(path, dataUI->projectName);

	strcat(pathFile, path);
	strcat(pathFile, "/users.csv");
	FILE *usersFile = fopen(pathFile, "w");

	User *curr = users;
	while(curr!=NULL) {
		fprintf(usersFile, "%d,%s,%s,%s,%s,%s,%s\n", curr->id, curr->name, curr->surname, curr->pesel, curr->adress, curr->email, curr->phone);
		curr = curr->next;
	}

	fclose(usersFile);

	strcpy(pathFile,"");
	strcat(pathFile, path);
	strcat(pathFile, "/books.csv");
	FILE *booksFile = fopen(pathFile, "w");

	Book *currB = books;
	while(currB!=NULL) {
		fprintf(booksFile, "%d,%s,%s,%s,%s,%d\n", currB->id, currB->name, currB->author, currB->isbn, currB->year, currB->available);
		currB = currB->next;
	}

	fclose(booksFile);

	strcpy(pathFile,"");
	strcat(pathFile, path);
	strcat(pathFile, "/borrowed_books.csv");
	FILE *borrowed_books_file = fopen(pathFile, "w");

	Borrowed_books *currBB = borrowed_books;
	while(currBB!=NULL) {
		fprintf(borrowed_books_file, "%d,%d,%s,%sd\n", currBB->user_id, currBB->book_id, currBB->borrow_date, currBB->return_date);
		currBB = currBB->next;
	}

	fclose(borrowed_books_file);
}

void SaveIcon(GtkWidget *parent) {
	GtkWidget *saveIcon = Image(parent, "SaveProject", "save.svg");

	GtkGestureClick *click_gesture = gtk_gesture_click_new();
	g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_SaveIcon_click), NULL);
	gtk_widget_add_controller(saveIcon, GTK_EVENT_CONTROLLER(click_gesture));

}

void on_LogoWrapper_click(){
	// GtkWidget *child = gtk_widget_get_first_child(root);
  // while (child != NULL) {
	// 	GtkWidget *next = gtk_widget_get_next_sibling(child);
	// 	gtk_box_remove(root, child);
	// 	child = next;
  // }

	// users=NULL;
	// books=NULL;
	// dataUI=NULL;
	// projects=NULL;

	// layoutWelcome(root);
}

void Logo(GtkWidget *parent){
	GtkWidget *logoWrapper = Div(parent, "LogoWrapper", "v", "", 0);
  Text(logoWrapper, "Logo", "B", 0.5);

	GtkGestureClick *click_gesture = gtk_gesture_click_new();
	g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_LogoWrapper_click), NULL);
	gtk_widget_add_controller(logoWrapper, GTK_EVENT_CONTROLLER(click_gesture));
}

GtkWidget *TopBar(GtkWidget *parent) {
  GtkWidget *topBar = Div(parent, "TopBar", "h", "", 0);
	Logo(topBar);

	GtkWidget *projectNameAndSave = Div(topBar, "ProjectNameAndSave", "h","", 0);
	projectName = Text(projectNameAndSave, "ProjectName", dataUI->projectName, 0);
	gtk_widget_set_hexpand(projectName, TRUE);
	
	SaveIcon(projectNameAndSave);

	GtkWidget *topBarOptions = Div(topBar, "TopBarOptions", "h","h", 16);
	// Text(topBarOptions, "Edit", "Edytuj", 0);
	GtkWidget *removeBookBtn = Text(topBarOptions, "Remove", "Usuń", 0);
	GtkGestureClick *click_gesture = gtk_gesture_click_new();
	g_signal_connect(click_gesture, "pressed", G_CALLBACK(item_remove), NULL);
	gtk_widget_add_controller(removeBookBtn, GTK_EVENT_CONTROLLER(click_gesture));

  return topBar;
}