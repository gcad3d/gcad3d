//  Simple-Gtk-Demo  (minimum)

#include "MS_Def1.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/ut_umem.h"                // Memspc MemObj UME_*


#include "../gui/gui__.h"


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


  static MemObj win0;


//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...

  TX_Print("gCad_main DemoGtk1");

  win1__ (NULL, GUI_SETDAT_E (UI_FuncInit));  // create window

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("...... gCad_fini DemoGtk1");

  GUI_Win_kill (&win0);

  AP_User_reset ();                      // close application

  return 0;

}

//=========================================================
  int win1__ (MemObj *mo, void **data) {
//=========================================================

  MemObj box0;


  // printf("win1__ %d\n",INT_PTR(data));
  printf("win1__ %d\n",GUI_DATA_EVENT);



  switch (GUI_DATA_EVENT) {

    case UI_FuncInit:
      win0 = GUI_Win__ ("Gtk1", win1__, "");
      box0 = GUI_box_v (&win0, "");
      GUI_button__ (&box0, "Test Button", win1__, &GUI_FuncUCB1, "");
      GUI_Win_go (&win0);
      break;

    case TYP_EventPress:                    // 99
      if(GUI_DATA_I1 == UI_FuncUCB1) {
        printf(" do UI_FuncUCB1\n");
      }
      break;

    case TYP_EventExit:     // from GUI_Win__
      gCad_fini ();
      break;

  }

  return 0;

}


//================  EOF  ==================================
