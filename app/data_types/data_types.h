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

// Data
typedef enum { USERS,BOOKS, DASHBOARD } CurrWindowType;

typedef struct {
    CurrWindowType currWindow;
    struct User *selectedUser;
    // Book *selectedBook;
} DataUI;

extern DataUI *dataUI;

void dataUI_init();
void dataUI_set_selectedUser(User *user);