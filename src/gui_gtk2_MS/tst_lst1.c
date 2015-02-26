/* tst_ed1.c


make run -f tst_lst1.mak

export LD_LIBRARY_PATH=.
cd ../binLinux32 && gdb a.out

export LD_LIBRARY_PATH=../binLinux32 && gdb ../binLinux32/a.out
make -f c-kit-gui-gtk.mak && make run -f tst_lst1.mak

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_umem.h"
#include "../gui/gui__.h"

#include "../gui_gtk2/gui_ut.h"

static MemObj  wLst = GUI_OBJ_NEW;




//================================================================
  int cb_lst (MemObj *mo, void **data) {
//================================================================


  printf(" mouseButtNr=%d selColNr=%d\n",GUI_DATA_I1,GUI_DATA_I2);
  printf(" txt sel |%s|\n",GUI_DATA_S3);


}


//================================================================
  int cb_f1 (MemObj *mo, void **data) {
//================================================================
// called when button is clicked ..

  static long  cpos=0, lNr=0;
  long         l1, l2, l3;
  char         *cp1, s1[256];

  cp1 = GUI_DATA_S1;
  printf("cb_f1 |%s|\n",cp1);


  //----------------------------------------------------------------
  if(!strcmp(cp1, "loaFil")) {
    GUI_list1_add_f (&wLst, "list1.dat", "1");
  }


  //----------------------------------------------------------------
  if(!strcmp(cp1, "clear")) {
    GUI_list1_clear (&wLst);
  }


  //----------------------------------------------------------------
  if(!strcmp(cp1, "dummy-")) {
    printf(" do not set focus -> editor \n");
    return 0;
  }


  //----------------------------------------------------------------
  // GUI_edi_Focus (&wLst); // else focus on button ..

  return 1;

}


//================================================================
  int UI_win_main (MemObj *mo, void **data) {
//================================================================
 
  static MemObj win0;

  int    ifunc;
  MemObj boxv0, boxv1;


  ifunc = GUI_DATA_I1;

  printf("<<<<<<<<<< UI_win_main %d\n",ifunc);


  switch (ifunc) {

    //---------------------------------------------------------
    case UI_FuncInit: 

      // Create Mainwindow
      win0 = GUI_Win__ ("Gtk-ed-test1", GUI_main_quit, "-300,-200");

      // get a hor. box in window
      boxv0 = GUI_box_v (&win0, "");
      boxv1 = GUI_box_h (&boxv0, "");

      // button in box
      GUI_button__ (&boxv1, "loaFil", cb_f1, (void*)"loaFil", "");
      GUI_button__ (&boxv1, "clear", cb_f1, (void*)"clear", "");
      GUI_button__ (&boxv1, "dummy-", cb_f1, (void*)"dummy-", "");


      // boxv1 = GUI_box_h (&boxv0, 0, 0);

      // GUI_button__ (&boxv1, "selGet", cb_f1, (void*)"selGet", 0, 0);

      wLst = GUI_list1__ (&boxv0, "list1.dat", "1", NULL, cb_lst, "");


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
