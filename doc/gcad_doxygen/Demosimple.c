//  Demosimple.c                   Simple-DLL-Demo  (minimum)


#include "MS_Def1.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_TX.h"          // TX_Print





#ifdef _MSC_VER
// export this functions
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// import functions exported from the core (see gCAD3D.def)
#define extern __declspec(dllimport)
#endif


// ext aus xa.c:
extern  char      WC_modnam[128];  // der Modelname





//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...


  printf("gCad_main Demosimple 1\n");

  // write to Main-Infowindow ..
  TX_Print("gCad_main out of Demosimple.dll");

  // get modelname from Mainprog
  printf("Modelname = %s\n",WC_modnam);


  // finish application
  gCad_fini ();

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  printf("gCad_fini Demosimple\n");

  // write to Main-Infowindow ..
  TX_Print("...... gCad_fini Demosimple");

  AP_User_reset ();     // close application

  return 0;

}
//================  EOF  ==================================
