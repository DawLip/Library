#include <stdlib.h>

#include "./data_types.h"
#include "../components/components.h"

DataUI *dataUI = NULL;

void dataUI_init() {
    DataUI *dUI = (DataUI *)malloc(sizeof(DataUI));
    dUI->currWindow = USERS;
    dUI->selectedUser = NULL;
    dataUI = dUI;
}

void dataUI_set_selectedUser(User *user) {
    dataUI->selectedUser = user;

    Workspace_rerender();
}