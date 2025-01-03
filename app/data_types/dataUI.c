#include <stdlib.h>

#include "./data_types.h"
#include "../components/components.h"

DataUI *dataUI = NULL;

void dataUI_init(char *libraryName){
    DataUI *dUI = (DataUI *)malloc(sizeof(DataUI));
    
    dUI->currWindow = USERS;
    dUI->selectedUser = NULL;
    dUI->selectedBook = NULL;
    strcpy(dUI->projectName, libraryName);

    dataUI = dUI;
}

void dataUI_set_selectedUser(User *user){
    dataUI->selectedUser = user;

    Workspace_rerender();
}

void dataUI_set_selectedBook(Book *book){
    dataUI->selectedBook = book;

    Workspace_rerender();
}

void dataUI_set_currWindow(CurrWindowType currWindow){
    dataUI->currWindow = currWindow;

    Hierarchy_rerender();
    Workspace_rerender();
}