#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./data_types.h"
#include "../components/components.h"

User *users;

void users_add(char *name, char *surname, char *pesel, char *adress, char *email, char *phone) {
    User *curr = users;
    User *new_user;
    new_user =(User *)malloc(sizeof(User));
    
    strcpy(new_user->name, name);
    strcpy(new_user->surname, surname);
    strcpy(new_user->pesel, pesel);
    strcpy(new_user->adress, adress);
    strcpy(new_user->email, email);
    strcpy(new_user->phone, phone);
    
    new_user->next = NULL;

    if(users==NULL) {
        new_user->id = 0;
        users = new_user;
        return;
    }

    while(curr->next != NULL) curr = curr->next;
    new_user->id = curr->id+1;
    curr->next = new_user;
}

void users_print(){
    User *curr = users;
    printf("Users:\n");
    while(curr != NULL) {
        printf("%d: %s %s %s\n", curr->id, curr->name, curr->surname, curr->pesel);
        curr = curr->next;
    }
    printf("\n");
}

void users_load_csv() {
    FILE *file = fopen("./app/users.csv", "r");
    char line[256];
    char *name;
    char *surname;
    char *pesel;
    char *adress;
    char *email;
    char *phone;

    if(file == NULL) { perror("Nie można otworzyć pliku"); return; }

    while(fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        name = strtok(line, ",");
        surname = strtok(NULL, ",");
        pesel = strtok(NULL, ",");
        adress = strtok(NULL, ",");
        email = strtok(NULL, ",");
        phone = strtok(NULL, ",");
        users_add(name, surname, pesel, adress, email, phone);
    }

    fclose(file);
}

void users_remove(GtkWidget *button, gpointer user_data) {
    User *curr = users;
    User *selectedUser = dataUI->selectedUser;

    if(curr == selectedUser) users=users->next;
    else {
        while(curr->next != selectedUser) curr = curr->next;

        User *next = curr->next;
        curr->next = next->next;
    }

    dataUI->selectedUser = NULL;

    Hierarchy_rerender();
    Workspace_rerender();
}