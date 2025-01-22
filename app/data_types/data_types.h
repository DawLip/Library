#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

// User
typedef struct {
    int id;

    char name[50];
    char surname[50];
    char pesel[13];
    char adress[100];
    char email[50];
    char phone[20];
    // struct User *prev;
    struct User *next;
} User;

extern User *users;

void users_add(int id, char *name, char *surname, char *pesel, char *adress, char *email, char *phone);
void users_load_csv();
void users_remove(GtkWidget *button, gpointer user_data);

//Book
typedef struct {
    int id;

    char name[200];
    char author[50];
    char isbn[14];
    char year[5];
    int available;

    struct Book *next;
} Book;

extern Book *books;

void books_add(int id, char *name, char *author, char *isbn, char *year, int available);
void books_load_csv();
void books_remove(GtkWidget *button, gpointer user_data);

// Models
typedef struct {
	char name[50];
    char decription[500];

	struct Model *next;
} Model;

extern Model *models;

void models_add(char *name, char *decription);
void models_print();
void models_load_csv();

// DataUI
typedef enum { USERS,BOOKS, DASHBOARD, MODELS } CurrWindowType;

typedef struct {
    char projectName[100];

    CurrWindowType currWindow;
    struct User *selectedUser;
    struct Book *selectedBook;
    struct Model *selectedModel;
} DataUI;

extern DataUI *dataUI;

void dataUI_init(char *libraryName);
void dataUI_set_currWindow(CurrWindowType currWindow);
void dataUI_set_selectedUser(User *user);
void dataUI_set_selectedBook(Book *book);
void dataUI_set_selectedModel(Model *model);

// Borrowed_books
typedef struct {
    int user_id;
    int book_id;
    char borrow_date[20];
    char return_date[20];

    struct Borrowed_books *next;
} Borrowed_books;

extern Borrowed_books *borrowed_books;

void borrowed_books_add(int user_id,int book_id, char *borrow_date, char *return_date);
void borrowed_books_print();
void borrowed_books_csv();


// Projects
typedef struct {
	char name[50];
	char date[50];

	struct Project *next;
} Project;

extern Project *projects;

struct Project* projects_add(char *name, char *date);