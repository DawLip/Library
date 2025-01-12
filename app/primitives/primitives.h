GtkWidget *Div(GtkWidget *parent, char* name, char* orientation, char* expand, int gap);
GtkWidget *DivS(GtkWidget *parent, char* name, char* orientation, char* expand, int gap);
GtkWidget *Image(GtkWidget *parent, char* name, char* src);

GtkWidget *Text(GtkWidget *parent, char* name, char* txt, float align);

GtkWidget *Input(GtkWidget *parent, char* value, char *placeholder, void (*func)(), int size);