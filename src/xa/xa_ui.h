/* ../xa/xa_ui.h

#include "../xa/xa_ui.h"               // APF_*
Needs
#include "../gui_gtk#/gtk_base.h"      // GUI_SETDAT_ES


Modif:
2001-06-14 Neu zu. RF.

*/


#include "../xa/xa_uid.h"        //  UID_WinMain ...
#include "../gui/gui_types.h"    // TYP_EventEnter


// #define UI_men__(txt) UI_menCB(NULL, GUI_SETDAT_ES(TYP_EventEnter,txt));
#define UI_but__(txt) UI_butCB(NULL, GUI_SETDAT_ES(TYP_EventEnter,txt));
#define UI_view__(txt) UI_viewCB(NULL, GUI_SETDAT_ES(TYP_EventEnter,txt));

// 0=OFF, 1=ON             Interactiviy is ON or OFF
#define INTACT_IS_ON ((AP_stat.iActStat)&&(UI_InpMode==UI_MODE_VWR))



// Application-Functions for UI_func_stat_set__ UI_func_stat_set_tab
enum { APF_EndTab,    // 0 = end of list
  APF_TB_CAD,         // Toolbar CAD
  APF_MEN0,           // gesamter oberer Menubar, von File bis Help !
  APF_MEN_FIL,        // Menu-File
  APF_MEN_SM,         // Menu-subModels
  APF_MEN_MDF,        // Menu-Modify
  APF_MEN_INS,        // Insert im oberen Menubar
  APF_MEN_SEL,        // Select im oberen Menubar
  APF_MEN_CAT,        // Catalog im oberen Menubar
  APF_MEN_APP,        // Menu-Applications
  APF_WIN_B_E,        // Browser-window & Editor-window
  APF_HIDE,           // HIDE & VIEW - Box
  APF_VWCADMAN,       // Box VWR / CAD / MAN
  APF_VWR,            // Mainfunc VWR
  APF_CAD,            // Mainfunc CAD
  APF_MAN,            // Mainfunc MAN
  APF_BUT_GSE,        // Buttons GO STEP END
  APF_BUT_GO,
  APF_BUT_STEP,
  APF_BUT_END,
  APF_PLUGINS,        // Plugins (userPrograms)
  APF_UNDO,           // Undo/Redo
  APF_MEASEA,         // Box Measure / Search/Name
  APF_MEN_PRC,        // Menu-Process
  APF_MEN_RPC,        // Menu-Remote
  APF___              // terminates list
};



//========= Prototypen: ============================
   int UI_butCB     (MemObj *mo, void **data);
   int UI_menCB     (MemObj *mo, void **data);
 
  void UI_AP        (int func, int widgetID, void* data);
  // int UI_upd_Refs  ();
  // void UI_mBars_off ();
 
  int  UI_win_main (MemObj *mo, void **data);

  void UI_Ed_sel    (long l_start, long l_end);
  void UI_Ed_ins    (int cpos, char *txt, int cNr);
  void UI_Ed_del    (int l_start, int l_end);
   int UI_Ed_hili   (int mode);
   int UI_EdKeyCR   ();
   // int UI_disp_vec1 (int typ, void *data, Point *pos);
 
 
  int UI_open__    (char *fnam,char *dirNam);
  // int UI_openCB    (GtkWidget *parent, void *data);
// int UI_WinIgeImp (GtkWidget *parent, void *data);
// int UI_WinDxfImp (GtkWidget *parent, void *data);
  int UI_WinAppr   (MemObj *mo, void **data);
  // int UI_WinInfo1  (MemObj *mo, void **data);
  int UI_WinPrint1 (MemObj *mo, void **data);
  int UI_WinToler  (MemObj *mo, void **data);
  // int UI_creObjHdr (GtkWidget *parent, void *data);
  // int UI_sur_Z_CB  (GtkWidget *widget, GdkEventKey *event);
  double UI_vwz__     (int mode, long ind);

  // int UI_CAD_activate ();
  int UI_wireCB    (MemObj *mo, void **data);
  // int UI_impDxfCB  (GtkWidget *parent, void *data);
  // int UI_impIgeCB  (GtkWidget *parent, void *data);
  // int UI_expDxfCB  (GtkWidget *parent, void *data);
  // int UI_expIgeCB  (GtkWidget *parent, void *data);
  int UI_save_ ();
 
  int UI_askExit   (MemObj *mo, void **data);
  int UI_askEscape ();
  int UI_file_sensi (int mode);


  int UI_hili_actLn ();

  int UI_func_stat_set__ (int i0, ...);
  int UI_func_stat_set_tab (short *sTab);

  char* AP_UserAppObjNam__ (int iNam);

//============ EOF ============= 
