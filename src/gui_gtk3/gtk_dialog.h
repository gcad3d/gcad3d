

// Standarddialogs
int    GUI_MsgBox      (char* text);

void   GUI_DialogYN    (char*, void*);
int    GUI_GetText     (char *text, char *label, int, void*);
int    GUI_GetTextCB   (void*, void*);

int    GUI_Color_select (int *ir, int *ig, int *ib);

int    GUI_DialogYN_CB  (void *parent, void *data);

void*  GUI_Slider__     (void *parent, int typ, void *funcnam,
                        double valMin, double valMax, double valStart,
                        int siz);
double GUI_Slider_get   (void* wsl1);
void   GUI_Slider_set   (void* wsl1, double newVal);

// INTERNAL:
 int GUI_File_cb    (void *parent, void *event, void *data);
 int GUI_Slider_cb  (void *parent, void *data);

// eof
