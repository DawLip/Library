#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./data_types.h"

Project *projects;

struct Project* projects_add(char *name, char *date) {
    Project *curr = projects;
    Project *new_project;
    new_project =(Project*)malloc(sizeof(Project));
    
    strcpy(new_project->name, name);
    strcpy(new_project->date, date);

    new_project->next = NULL;

    if(books==NULL) {
        projects = new_project;
        return;
    }

    while(curr->next != NULL) curr = curr->next;
    curr->next = new_project;

		return new_project;
}