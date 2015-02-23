
  MemObj GUI_menu__ (MemObj *o_par, char *ltxt, int mode);
  MemObj GUI_menu_entry (MemObj *o_par, char* ltxt,
                         void *funcnam, void *data);

  MemObj GUI_menu_checkbox__ (MemObj *o_par, char* ltxt, int state,
                              void *funcnam, void *data);
  int GUI_menu_checkbox_set (MemObj *mo, int mode);
  int GUI_menu_checkbox_get (MemObj *mo);

  MemObj GUI_menu_radiobutt__ (MemObj *o_par, char* ltxt, int ii,
                               void *funcnam, void *data);
  int GUI_menu_radiobutt_get (MemObj *mo);
  void GUI_menu_radiobutt_set (MemObj *mo);


// EOF
