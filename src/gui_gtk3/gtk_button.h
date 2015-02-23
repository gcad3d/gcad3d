


MemObj GUI_button__     (MemObj *o_par, char* ltext,
                         void *funcnam, void *data, char *siz);
int    GUI_button_mod   (MemObj *wb, char* ltext);
int    GUI_button_styl  (MemObj *mo, int iCol);

  MemObj GUI_button_img (MemObj *o_par, char* filnam,
                         void *funcnam, void *data, char *opts);

int GUI_ckbutt_get (MemObj *mo);
int GUI_ckbutt_set (MemObj *mo, int mode);
int GUI_ckbutt_set_noCB (MemObj *mo, int mode);
MemObj GUI_ckbutt__ (MemObj *o_par, char* ltxt, int state,
                       void *funcnam, void *data, char *siz);

//.............................
// INTERNAL:
int    GUI_button_press (void *parent, MemObj data);


// EOF
