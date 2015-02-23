/* ../gui_gtk2/gtk_base.h


Modif:
2012-03-18 New; extracted from ut_gtk. RF.

*/



void    GUI_Init__      (char *fRC);

MemObj  GUI_Win__       (char *wtit, void *funcnam, char *opts);
int     GUI_Win_go      (MemObj  *o_par);
int     GUI_Win_ev_key  (MemObj  *o_par, void *fKey);
int     GUI_Win_ev_button (MemObj  *o_par, void *fButton);
int     GUI_Win_up      (MemObj  *parentWin, MemObj  *o_win, int typ);
char*   GUI_Win_tit     (void *gtkWin);
void    GUI_Win_kill    (MemObj *mo);
int     GUI_main_quit   ();

MemObj GUI_box_h (MemObj *o_par, char *siz);
MemObj GUI_box_v (MemObj *o_par, char *siz);
MemObj GUI_box__ (MemObj *o_par, int typ, char *siz);
int GUI_w_pack_d (int *isx, int *isy, int *iex, int *iey, char *siz);
int GUI_w_pack_b (int pTyp, void *pObj, void *cObj,
                    int *isx, int *isy, int *iex, int *iey);
int GUI_w_pack2 (int pTyp, void *pObj, void *cObj, char *siz);
int GUI_w_pack1 (int pTyp, void *pObj, void *cObj, char *siz);


// #define GUI_box_h(o_par,hsiz,vsiz) GUI_box__(o_par,0,hsiz,vsiz)
// #define GUI_box_v(o_par,hsiz,vsiz) GUI_box__(o_par,1,hsiz,vsiz)
// MemObj  GUI_box__       (MemObj *o_par, int typ, int hsiz, int vsiz);

#define GUI_spc_h(o_par,hsiz) GUI_spc__(o_par,0,hsiz)
#define GUI_spc_v(o_par,vsiz) GUI_spc__(o_par,1,vsiz)
void    GUI_spc__       (MemObj *o_par, int typ, int spc);

#define GUI_sep_h(o_par,hsiz) GUI_sep__(o_par,0,hsiz)
#define GUI_sep_v(o_par,vsiz) GUI_sep__(o_par,1,vsiz)
void    GUI_sep__       (MemObj *o_par, int typ, int border);


int     GUI_is_alive    (int winId);
int     GUI_get_keys_mod    ();
int     GUI_update__    ();

#define GUI_hide__(o_par) GUI_set_show(o_par,0)
#define GUI_show__(o_par) GUI_set_show(o_par,1)
int     GUI_set_show    (MemObj *mo, int mode);
int     GUI_set_enable  (MemObj *mo, int mode);


  int GUI_idle__ (void *funcnam, void *data);
  int GUI_timer__ (void *funcnam, int interval, void *data);



// INTERNAL:
  int GUI_Win_exit (void *parent, MemObj um);
  char* GUI_Win_tit (void *gtkWin);
  int GUI_Win_key (void *parent, void *event, void *data);
  int GUI_Win_button (void *parent, void *ev, void *data);
  int GUI_packwn (void *parent, void *wi, int hsiz, int vsiz);
  int GUI_siz_set (void *parent, int hSiz, int vSiz);
  int GUI_cursor__ (MemObj *mo, int iCur);

  void** GUI_SETDAT_E  (int ie);
  void** GUI_SETDAT_EI (int ie, int ii);
  void** GUI_SETDAT_ES (int ie, char *is);

  int GUI_OBJ_TYP (MemObj *mo);
  int GUI_OBJ_SRC (MemObj *mo);


// EOF
