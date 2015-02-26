// gtk_ed.h 


#ifdef GTK1
// MS-Win: GTK1

typedef struct {GtkWidget *view;
                GtkWidget *text;
                GtkWidget *win;
                int stat;}                                        GIO_WinEd;

#else
// Linux: GTK2

typedef struct {GtkWidget *view;
                GtkTextBuffer *text;
                GtkWidget *win;
                int stat;}                                        GIO_WinEd;
  // view for focus, text for write, win for pack. stat=0=off, 1=on.
#endif



typedef struct {GtkWidget *win; int stat;}                        GIO_Win__;
  // win for focus, write, pack. stat=0=off, 1=on.





// int  GUI_Ed_sel__    (GtkWidget *text, int von, int bis);
// int  GUI_Ed_sel_del  (GtkWidget *text);

// int  GUI_Ed_del      (GtkWidget *text, int von, int bis);

// int  GUI_Ed_getLsta  (GtkWidget *text, int cPos);
// int  GUI_Ed_getLend  (GtkWidget *text, int cPos);
// int  GUI_Ed_getLpos  (int *sPos, int *ePos, int cPos, GtkWidget *text);
long GUI_Ed_getCpos  (GIO_WinEd *text);
// long GUI_Ed_getEof   (GtkWidget *text);
// int  GUI_Ed_setCpos  (GtkWidget *text, long cpos);
// int  GUI_Ed_setCnxtL (GtkWidget *text);

// int  GUI_Ed_scroll_u (GtkWidget *text, int cPos);

// int  GUI_Ed_Read     (GtkWidget *text, char* txbuf, long *txlen);
// int  GUI_Ed_RdLnAct  (GtkWidget *text, char* txbuf, int *txlen);
// char GUI_Ed_RdChr    (GtkWidget *text, int offset);

// int  GUI_Ed_Insert   (GtkWidget *text, int cpos, char* txbuf, long txlen);
int  GUI_Ed_Write    (GIO_WinEd *text, char* txbuf, long *txlen, long maxlen);

void GUI_Ed_Focus    (GIO_WinEd *wTx);

// GtkWidget* GUI_Ed_Init (GtkWidget **wtext, void* funcnam, int mode);
  void GUI_Ed_Init (void *parent, GIO_WinEd *wTx, void* funcnam, int mode);
// void GUI_Ed_Init     (GIO_WinEd *text, void* funcnam, int mode);
// void GUI_Ed_Init1    (GIO_WinEd *text);

/* ====================== EOF =========================== */
