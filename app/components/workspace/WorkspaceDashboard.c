#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>

#include "../components.h"
#include "../../primitives/primitives.h"
#include "../../data_types/data_types.h"

void on_borrow_click() {
  User *user = dataUI->selectedUser;
  Book *book = dataUI->selectedBook;

  char date[11];
  time_t now = time(NULL);
  struct tm *tm_now = localtime(&now);
  strftime(date, 11, "%Y-%m-%d", tm_now);

  borrowed_books_add(user->id, book->id, date, "-");

  Workspace_rerender();
}

void on_return_click() {
  User *user = dataUI->selectedUser;
  Book *book = dataUI->selectedBook;

  char date[11];
  time_t now = time(NULL);
  struct tm *tm_now = localtime(&now);
  strftime(date, 11, "%Y-%m-%d", tm_now);

  Borrowed_books *curr = borrowed_books;
  while(!(curr->book_id == book->id && curr->user_id == user->id && strcmp(curr->return_date,"-")==0)) 
    curr = curr->next;

  strcpy(curr->return_date, date);

  Workspace_rerender();
}

GtkWidget *Button(GtkWidget *parent, char *label, void (*func)()) {
  GtkWidget *button = Div(parent, "Button", "v", "vh", 16);

  Text(button, "ButtonLabel", label, 0);

  GtkGestureClick *click_gesture = gtk_gesture_click_new();
    g_signal_connect(click_gesture, "pressed", G_CALLBACK(func), NULL);
    gtk_widget_add_controller(button, GTK_EVENT_CONTROLLER(click_gesture));

  return button;
}

GtkWidget *WorkspaceDashboard(GtkWidget *parent) {
  GtkWidget *workspaceDashboard = Div(parent, "Workspace", "v", "vh", 16);

  User *user = dataUI->selectedUser;
  Book *book = dataUI->selectedBook;

  GtkWidget *selectedUser = Div(workspaceDashboard, "Selected", "h", "", 8);
  Text(selectedUser, "SelectedLabel", "Wybrany użythownik:", 0);
  if(user!=NULL) {
    char user_id[10];
    sprintf(user_id, "%d", user->id);

    Text(selectedUser, "SelectedItemTxt", user_id, 0);
    Text(selectedUser, "SelectedItemTxt", user->name, 0);
    Text(selectedUser, "SelectedItemTxt", user->surname, 0);
  } else Text(selectedUser, "SelectedItemTxt", "wybierz użytkownika", 0);

  GtkWidget *selectedBook = Div(workspaceDashboard, "Selected", "h", "", 8);
  Text(selectedBook, "SelectedLabel", "Wybrana książka:", 0);
  if(book!=NULL) {
    char book_id[15];
    sprintf(book_id, "%d", book->id);

    Text(selectedBook, "SelectedItemTxt", book_id, 0);
    Text(selectedBook, "SelectedItemTxt", book->name, 0);
  } else Text(selectedBook, "SelectedItemTxt", "wybierz książkę", 0);


  GtkWidget *actions = Div(workspaceDashboard, "Actions", "h", "", 8);
  Text(actions, "ActionsLabel", "Akcje: ", 0);
  if(user!=NULL && book!=NULL) {
    Borrowed_books *curr = borrowed_books;
    while(curr!=NULL && !(curr->book_id == book->id && curr->user_id != user->id && strcmp(curr->return_date,"-")==0)) 
      curr = curr->next;
    if(curr!=NULL){
      Text(actions, "SelectedItemTxt", "Książka jest aktualnie wypożyczona przez innego czytelnika", 0);
    } else {
      curr = borrowed_books;
      while(curr!=NULL && !(curr->book_id == book->id && curr->user_id == user->id && strcmp(curr->return_date,"-")==0)) 
        curr = curr->next;
      if(curr==NULL) Button(actions, "Wypożycz", on_borrow_click);
      else Button(actions, "Oddaj", on_return_click);
    }
  } else Text(actions, "SelectedItemTxt", "Wybierz książkę i urzytkownika aby zobaczeć dostępne akcje", 0);
  
  
  return workspaceDashboard;
}