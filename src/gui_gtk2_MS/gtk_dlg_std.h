

// Standarddialogs

int    GUI_DialogEntry  (char* label, char *entry, int eSiz,
                         char *buttons[], int border);
int    GUI_Dialog_2b    (char *txt, char *tb1, char *tb2);
int    GUI_Dialog_e2b   (char *txt, char *entry, int eSiz, char *tb1, char *tb2);

// int    GUI_MsgBox      (char* text);

// void   GUI_DialogYN    (char*, void*);
// int    GUI_GetText     (char *text, char *label, int, void*);
// int    GUI_GetTextCB   (void*, void*);

int    GUI_Color_select (int *ir, int *ig, int *ib);

// int    GUI_DialogYN_CB  (void *parent, void *data);

MemObj GUI_Slider__ (MemObj *o_par, int typ, void *funcnam,
                     double valMin, double valMax, double valStart,
                     char *opts);
double GUI_Slider_get   (MemObj* wsl1);
void   GUI_Slider_set   (MemObj* wsl1, double newVal);

// INTERNAL:
 // int GUI_File_cb    (void *parent, void *event, void *data);
int GUI_Slider_cb1 (void *parent, MemObj mo);
int GUI_Slider_cb2 (void *parent, void *ev, void *data);
int GUI_DialogEntryCB (void *parent, void **data);

// eof
