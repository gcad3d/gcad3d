
MemObj GUI_entry__   (MemObj *o_par, char* ltxt, char* etxt,
                      void *funcnam, void *data, char *siz);

int    GUI_entry_set (MemObj *mo, char* text);
char*  GUI_entry_get (MemObj *mo);
int    GUI_entry_copy (char *cbuf, int sSiz, MemObj *mo);
int    GUI_entry_sel_del (MemObj *mo);


// INTERNAL:
int    GUI_entry_cb  (void *parent, void *event, MemObj mo);

// EOF
