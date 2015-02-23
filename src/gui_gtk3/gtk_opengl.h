

MemObj GUI_gl__ (MemObj *o_par, void *fDraw, char *opts);

  int GUI_gl_ev_move   (MemObj *mo, void *fMove);
  int GUI_gl_ev_button (MemObj *mo, void *fButton);
  int GUI_gl_ev_key    (MemObj *mo, void *fKey);
  int GUI_gl_block     (MemObj *mo, int mode);

  int GUI_gl_draw     (void *parent, void *event, MemObj mo);
  int GUI_gl_move     (void *parent, void *event, MemObj mo);
  int GUI_gl_button   (void *parent, void *event, MemObj mo);
  int GUI_gl_key      (void *parent, void *event, MemObj mo);

  int GUI_gl_pending  ();
  int GUI_gl_size     (int *x, int *y);
  int GUI_gl_idle1    (void* data);

// EOF
