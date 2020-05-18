// Demo C++ - Program as plugin for gCAD3D         2011-04-04    RF
// works with Linux & MS-win.


// make -f Demo_CPP.mak
// cp Demo_CPP.so /p2/fwork/gCAD3D/binLinux32/plugins/.

// cc -c Demo_CPP.cpp
// cl -c /EHsc Demo_CPP.cpp


extern "C" {
#include "../ut/ut_geo.h"
#include "../ut/ut_TX.h"          // TX_Print
#include "../gr/ut_GL.h"          // GL_DrawPoly
#include "../gr/ut_DL.h"          // DL_Redraw
#include "../gr/ut_gr.h"          // GR_tDyn_pcv
//void TX_Print (char* txt, ...);
int AP_User_reset ();
}


#include <iostream>




#ifdef _MSC_VER
// export this functions
extern "C" {
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// import functions exported from the core (see gCAD3D.def)
#define extern __declspec(dllimport)
//__declspec(dllimport) int AP_User_reset ();
}
#else
extern "C" { int gCad_main (); }
extern "C" { int gCad_fini (); }

#endif


using namespace std;



//================================================================
int gCad_main () {
//================================================================

  TX_Print("...... Demo_CPP starting ......");
  cout << "Test3\n";


  long      dli = -1;                             // DispListIndex temp.
  Point     pa[4]={{0.0, 0.0, 0.0},
                   {100.0, 0.0, 0.0},
                   {100.0, 100.0, 0.0},
                   {200.0, 100.0, 10.0}};
  // GL_DrawPoly (&dli, 1, 4, pa);                   // create temporary polygon
  GR_tDyn_pcv (pa, 4, Typ_Att_dash_long);

  DL_Redraw ();                                   // redraw

  gCad_fini ();                                   // exit plugin

  return 0;

}


//================================================================
int gCad_fini () {
//================================================================

  TX_Print("...... exit Demo_CPP");
  AP_User_reset ();     // close application

  cout << "finito\n";
  return 0;

}

// EOF
