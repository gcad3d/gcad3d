
MemObj GUI_list1__         (MemObj *o_par, char *fNam,
                           char *mode, char *titLst[], void *funcnam, char* opts);
MemObj GUI_list2__         (MemObj *o_par, char *fNam,
                           char *mode, char *titLst[], void *funcnam, char *opts);
int GUI_list1_clear (MemObj *mo);
int GUI_list1_add_f (MemObj *mo, char *fNam, char *mode);
int GUI_list2_add_f (MemObj *mo, char *fNam, char *mode);



int    GUI_list1_h2__      (char *titP, char *titC, char *lButP,
                            char *txtP, char *fnamP,
                            void *funcP, void *funcC, char *opts);
int    GUI_list1_dlg_w     (char *sels, int sSiz,
                            void *parWin, char *winTit, char *fNam,
                            char *mode, char *titLst[], char *opts);
int    GUI_list1_dlg__     (MemObj *o_par, char *winTit, char *fNam, char *mode,
                            char *titLst[], void *funcnam, char *opts);
void   GUI_list1_dlg_del   ();

// internal:
int    GUI_list1_dlg_cbw   (Obj_gui2 *go, void **data);
int    GUI_list1_del       (void *parent, void *data);

int    GUI_list1_cbSel     (void *selection, MemObj mo);
int    GUI_list1_cbMouse   (void *parent, void *event, MemObj mo);
int    GUI_list1_cbKey     (void *parent, void *event, MemObj mo);
int    GUI_list1_f         (void *list_store, char *fnam, char *mode);
int    GUI_list1_cbw       (void *parent, void **data);

// EOF
