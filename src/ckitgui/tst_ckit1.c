/* tst_ckit1.c

Test menu


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_umem.h"
#include "../gui/gui__.h"


static MemObj  wLst = GUI_OBJ_NEW;




//================================================================
  int menCB__ (MemObj *mo, void **data) {
//================================================================
// callback menu

  char *cp1;

  cp1 = GUI_DATA_S1;

  printf("menCB__ |%s|\n",cp1);



  //-----------------------------------------------------
  if(!strcmp(cp1, "cb0")) {
    // checkBox changed
    printf(" checkbox -> %d\n",GUI_menu_checkbox_get(mo));
    goto L_exit;
  }

  //-----------------------------------------------------
  if(!strncmp(cp1, "rb", 2)) {
    // radioBox changed; skip disactivation ..
    if(GUI_DATA_EVENT == TYP_EventRelease) return 0;
    printf(" radiobuttons -> %s\n",cp1);
    goto L_exit;
  }


  //-----------------------------------------------------
  L_exit:
  return 0;

}

//================================================================
  int openCB__ (MemObj *mo, void **data) {
//================================================================
// callback menu

  char *cp1;

  cp1 = GUI_DATA_S1;

  printf("openCB__ |%s|\n",cp1);

  return 0;

}



//================================================================
  int callback_button1 (void *parent, void **data) {
//================================================================
// called when button is clicked ..

  printf("callback_button1\n");

  return 1;

}


//================================================================
  int UI_win_main (MemObj *mo, void **data) {
//================================================================

  static MemObj win0;

  int    ifunc;
  MemObj boxv0, wm0, wm1, wm13, wm2;


  ifunc = GUI_DATA_I1;

  printf("<<<<<<<<<< UI_win_main %d\n",ifunc);


  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit:

      // Create Mainwindow
      win0 = GUI_Win__ ("Gtk-test", GUI_main_quit, "-300,-200");

      // get a hor. box in window
      boxv0 = GUI_box_v (&win0, "");
      // boxv1 = GUI_box_h (&boxv0, "");


      //----------------------------------------------------------------
      // menu
      wm0 = GUI_menu__ (&boxv0, NULL, 0);            // the toolbar

      wm1 = GUI_menu__ (&wm0, "File ", 0);
      GUI_menu_entry (&wm1, "New",  menCB__,  (void*)"new");
      GUI_menu_entry (&wm1, "Open", menCB__,  (void*)"open");
      wm13 = GUI_menu__ (&wm1, "  last-used", 's');

        GUI_menu_entry (&wm13, "file1",  openCB__,  (void*)"file1");
        GUI_menu_entry (&wm13, "file2",  openCB__,  (void*)"file2");
        GUI_menu_entry (&wm13, "file3",  openCB__,  (void*)"file3");
        GUI_menu_entry (&wm13, "file4",  openCB__,  (void*)"file4");

      wm2 = GUI_menu__ (&wm0, "test", 0);


      GUI_menu_radiobutt__ (&wm2, "radioBt1", 0, menCB__,  (void*)"rb1");
      GUI_menu_radiobutt__ (&wm2, "radioBt2", 1, menCB__,  (void*)"rb2");
      GUI_menu_radiobutt__ (&wm2, "radioBt3", 2, menCB__,  (void*)"rb3");

      GUI_menu_checkbox__ (&wm2, "checkBox", 0, menCB__,  (void*)"cb0");

      //----------------------------------------------------------------
      // button in box
      GUI_button__ (&boxv0, "press me", callback_button1, NULL, "");

      // windowSetup finished; display it ..
      GUI_Win_go (&win0);

  }

  return 0;

}


//================================================================
  int main (int argc, char *argv[]) {
//================================================================

  MemObj window, boxv0;     // the widgets

  // init
  GUI_Init__ ("");

  UI_win_main (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

  return 0;

}


// EOF

