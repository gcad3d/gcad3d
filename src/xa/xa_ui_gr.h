// xa_ui_gr.h

/// test if hide or view is active; 1=yes, 0=no.
#define UI_CK_HIDE_VIEW (((UI_stat_hide==0)||(UI_stat_view==0))?1:0)


// OpenGL interface app2GL
void UI_GR_DrawInit ();
void UI_GR_DrawExit ();

int UI_GL_draw__  (MemObj *mo, void **data);
int UI_GL_move__  (MemObj *mo, void **data);
int UI_GL_mouse__ (MemObj *mo, void **data);
int UI_GL_keys__  (MemObj *mo, void **data);

void UI_GR_view_set_func ();
void UI_GR_view_set_Cen__  (int mode);
void UI_GR_view_set_Cen1   (double zVal);
int  UI_ChangeCursor (int newCur);
int  UI_ResetCursor ();
int  UI_GR_Select1 (int mode, long *dlInd);
int  UI_GR_Select2 (int typ, long dbi, long dli);
int  UI_GR_Select3 (long objInd);
int  UI_GR_Select_work1 (long objInd);
// int  UI_GR_Select_work2 (int typ, long ind, char* buf);
int  UI_GR_Select_work2 (int typ, long ind, char* buf, long dli);
int  UI_GR_GetdlInd (long *ind);
int  UI_GR_Indicate();
// int  UI_GR_view (void *parent, void *data);
// void UI_GR_Redraw (void *parent, void *data);
// void UI_GR_Redraw ();

  int UI_undo (void *parent, void *data);
  int EX_OpGetList (int *anz, char ***buf);

  int UI_CB_hide  (MemObj *mo, void **data);
  int UI_CB_view  (MemObj *mo, void **data);

  int AP_Mousemove2dx (double *dv, int dx, int dy);

  // void* UI_GR_Init (int width, int height);
  // void* UI_GR_WinInit (int width, int height, int border);

// int UI_GR_GLInit (void *widget);
// int UI_GR_Draw (void *widget, void *event);
// int UI_GR_Reshape (void *widget, void *event);
// int UI_GR_test1 (void *widget, void *event);
// int UI_GR_MotionNotify (void *widget, void *event);

// int UI_GR_idle_GL1_CB (void *data);
// int UI_GR_idle_GL2_CB (void *data);
// int UI_GR_idle_GL3_CB (void *data);

// int UI_GR_ButtonPress (void *widget, void *data);
// int UI_GR_ButtonRelease (void *widget, void *data);


// int UI_GR_Enter    (void *widget, void *data);
int UI_GR_Leave    (void *widget, void *data);
int UI_GR_KeyPress (void *widget, void *data);
int UI_GR_KeyRelease (void *widget, void *data);
// int UI_GR_Destroy   (void *widget);

int UI_viewCB (MemObj *mo, void **data);

int  UI_Do_Main (void *parent, void *data);


int CI_Mouse (int buttNr,int KeyStatShift,int KeyStatCtrl,int KeyStatAlt,
              int x,int y);
int CI_Key   (char ltxt, int KeyStatShift,int KeyStatCtrl,int KeyStatAlt);
int CI_FKey  (int key, int KeyStatShift,int KeyStatCtrl,int KeyStatAlt);

// int UI_GR_setKeyFocus ();

int UI_GR_get_actPos_ (char**);

int UI_KeyFieldWri ();


int AP_UserSelection_reset ();



/*======================== EOF ============================*/

