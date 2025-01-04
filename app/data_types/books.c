#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./data_types.h"

Book *books;

void books_add(int id, char *name, char *author, char *isbn, char *year, int available) {
    Book *curr = books;
    Book *new_book;
    new_book =(Book*)malloc(sizeof(Book));
    
    strcpy(new_book->name, name);
    strcpy(new_book->author, author);
    strcpy(new_book->isbn, isbn);
    strcpy(new_book->year, year);
    new_book->available=available;

    new_book->next = NULL;

    if(books==NULL) {
        new_book->id = 0;
        books = new_book;
        return;
    }

    while(curr->next != NULL) curr = curr->next;
    new_book->id = id;
    curr->next = new_book;
}

void books_print(){
    Book *curr = books;
    printf("Books:\n");
    while(curr!= NULL) {
        printf("%d: %s %s\n", curr->id, curr->name, curr->author);
        curr = curr->next;
    }
    printf("\n");
}

void books_load_csv(){
    char src[200]="";
    strcat(src, "./app/saves/");
    strcat(src, dataUI->projectName);
    strcat(src, "/books.csv");

    FILE *file = fopen(src, "r");
    char line[512];
    int id;
    char *name;
    char *author;
    char *isbn;
    char *year;
    int available;

    if(file == NULL) { perror("Nie można otworzyć pliku"); return; }

    while(fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        id = atoi(strtok(line, ","));
        name = strtok(NULL, ",");
        author = strtok(NULL, ",");
        isbn = strtok(NULL, ",");
        year = strtok(NULL, ",");
        available = atoi(strtok(NULL, ","));
        books_add(id, name, author, isbn, year, available);
    }

    fclose(file);
}

void books_remove(GtkWidget *button, gpointer user_data) {
    Book *curr = books;
    Book *selectedBook = dataUI->selectedBook;

    if(curr == selectedBook) books=books->next;
    else {
        while(curr->next != selectedBook) curr = curr->next;

        Book *next = curr->next;
        curr->next = next->next;
    }

    dataUI->selectedBook = NULL;

    Hierarchy_rerender();
    Workspace_rerender();
}