#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./data_types.h"

Book *books;

void books_add(char *name, char *author) {
    Book *curr = books;
    Book *new_book;
    new_book =(Book*)malloc(sizeof(Book));
    
    strcpy(new_book->name, name);
    strcpy(new_book->author, author);

    new_book->next = NULL;

    if(books==NULL) {
        new_book->id = 0;
        books = new_book;
        return;
    }

    while(curr->next != NULL) curr = curr->next;
    new_book->id = curr->id+1;
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
    FILE *file = fopen("./app/books.csv", "r");
    char line[512];
    char *name;
    char *author;

    if(file == NULL) { perror("Nie można otworzyć pliku"); return; }

    while(fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        name = strtok(line, ",");
        author = strtok(NULL, ",");
        if (name != NULL && author != NULL) {
            books_add(name, author);
        } else {
            printf("Błąd w parsowaniu linii: %s\n", line);
        }
    }

    fclose(file);
}