
MemObj  GUI_frame__ (MemObj *o_par, char* ltxt, int border);

MemObj  GUI_toolbox__ (MemObj *o_par);

MemObj  GUI_notebook__ (MemObj *o_par, void *funcnam);
MemObj  GUI_notebook_add (MemObj *o_ntb, char *title);
  int   GUI_notebook_get (MemObj *o_ntb);
  void  GUI_notebook_set (MemObj *o_ntb, int pgNr);

MemObj  GUI_box_paned__ (MemObj *box1, MemObj *box2,
                         MemObj *o_par, int mode, int resi, int siz1);


// INTERNAL:
  int   GUI_notebook_CB (void *ntb, void *nbp, int pgNr, MemObj mo);

// EOF
