// ../gui_gtk2/gtk_multichoice.h


  MemObj GUI_radiobutt__ (MemObj *o_par, char* ltxt, int ii,
                          void *funcnam, void *data, char *siz);
  int GUI_radiobutt_get (MemObj *mo);
  void GUI_radiobutt_set (MemObj *mo);
  int GUI_radiobutt_press (void *parent, MemObj mo);

  int GUI_popup__ (char *optLst[], char *tipLst[], int itip,
                   void *funcnam, void *dataLst[]);

  int GUI_popup_cb1 (void *parent, void *data);
  int GUI_popup_cb2 (void *parent, void *event, void *data);


  MemObj GUI_optmen__ (MemObj *o_par, char *mtxt,
                       char *optLst[], char *tipLst[],
                       void* funcnam, char *opts);
  int GUI_optmen_set (MemObj *mo, int mode, void *data);
  int GUI_optmen_chg (MemObj *mo,
                      char *optLst[], char *tipLst[]);

  int GUI_optmen_go (void *parent, void *event, MemObj mo);
  void GUI_optmen_pos (void* menu, int *x, int *y, int *ii, void *data);
  void GUI_optmen_sel (void *parent, void *data);


// EOF
