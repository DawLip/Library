#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./data_types.h"

Borrowed_books *borrowed_books;

void borrowed_books_add(int user_id,int book_id, char *borrow_date, char *return_date) {
    Borrowed_books *curr = borrowed_books;
    Borrowed_books *new_borrowed_book;
    new_borrowed_book =(Borrowed_books*)malloc(sizeof(Borrowed_books));
    
    new_borrowed_book->user_id = user_id;
    new_borrowed_book->book_id = book_id;
    strcpy(new_borrowed_book->borrow_date, borrow_date);
    strcpy(new_borrowed_book->return_date, return_date);

    new_borrowed_book->next = NULL;

    if(borrowed_books==NULL) {
        borrowed_books = new_borrowed_book;
        return;
    }

    while(curr->next != NULL) curr = curr->next;
    curr->next = new_borrowed_book;
}

void borrowed_books_print() {
    Borrowed_books *curr = borrowed_books;
    printf("Borrowed_books:\n");
    while(curr!= NULL) {
        printf("%d %d %s %s\n", curr->user_id, curr->book_id, curr->borrow_date, curr->return_date);
        curr = curr->next;
    }
    printf("\n");
}

void borrowed_books_csv() {
    char src[200]="";
    strcat(src, "./app/saves/");
    strcat(src, dataUI->projectName);
    strcat(src, "/borrowed_books.csv");

    FILE *file = fopen(src, "r");
    char line[512];
    int user_id;
    int book_id;
    char *borrow_date;
    char *return_date;

    if(file == NULL) { perror("Nie można otworzyć pliku"); return; }

    while(fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        user_id = atoi(strtok(line, ","));
        book_id = atoi(strtok(NULL, ","));
        borrow_date = strtok(NULL, ",");
        return_date = strtok(NULL, ",");
        borrowed_books_add(user_id, book_id, borrow_date, return_date);
    } 

    fclose(file);
}