#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./data_types.h"

Model *models;

void models_add(char *name, char *decription) {
    Model *curr = models;
    Model *new_model;
    new_model =(Model*)malloc(sizeof(Model));
    
    strcpy(new_model->name, name);
    strcpy(new_model->decription, decription);
    new_model->next = NULL;

    if(models==NULL) {
        models = new_model;
        return;
    }

    while(curr->next != NULL) curr = curr->next;
    curr->next = new_model;
}

void models_print(){
    Model *curr = models;
    printf("Models:\n");
    while(curr!= NULL) {
        printf("%s\n", curr->name);
        curr = curr->next;
    }
    printf("\n");
}

void models_load_csv(){
    char src[200]="";
    strcat(src, "./app/saves/");
    strcat(src, "/models.csv");

    FILE *file = fopen(src, "r");
    char line[512];
    char *name;
    char *description;

    if(file == NULL) { perror("Nie można otworzyć pliku"); return; }

    while(fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        name = strtok(line, "|");
        description = strtok(NULL, "|");
        models_add(name, description);
    }

    fclose(file);
}
