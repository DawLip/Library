#pragma once

#include <stdio.h>

// User
typedef struct {
    int id;

    char name[50];
    char surname[50];
    char pesel[12];

    // struct User *prev;
    struct User *next;
} User;

extern User *users;

void users_add(char *name, char *surname, char *pesel);
void users_load_csv();

//Book
typedef struct {
    int id;

    char name[50];
    char author[50];

    struct User *next;
} Book;

extern Book *books;

void books_add(char *name, char *author);
void books_load_csv();

// DataUI
typedef enum { USERS,BOOKS, DASHBOARD } CurrWindowType;

typedef struct {
    CurrWindowType currWindow;
    struct User *selectedUser;
    struct Book *selectedBook;
} DataUI;

extern DataUI *dataUI;

void dataUI_init();
void dataUI_set_currWindow(CurrWindowType currWindow);
void dataUI_set_selectedUser(User *user);
void dataUI_set_selectedBook(Book *book);