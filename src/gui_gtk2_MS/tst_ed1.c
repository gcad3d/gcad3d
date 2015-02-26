/* tst_ed1.c


make run -f tst_ed1.mak

export LD_LIBRARY_PATH=.
cd ../binLinux32 && gdb a.out

export LD_LIBRARY_PATH=../binLinux32 && gdb ../binLinux32/a.out

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#include "../ut/ut_umem.h"
#include "../gui/gui__.h"

#include "../gui_gtk2/gui_ut.h"

static MemObj  wEd;




//================================================================
  int ed_CB (MemObj *mo, void **data) {
//================================================================


  static long  lnrAct;


   printf("ed_CB event=%d typ=%d\n",GUI_DATA_EVENT,GUI_OBJ_TYP(mo));


  if(GUI_DATA_EVENT == TYP_EventEnter) {
    printf(" .. enter pos=%ld lnr=%ld;\n",GUI_DATA_L1,GUI_DATA_L2);
    if(lnrAct != GUI_DATA_L2) {
      printf(" XXXXX NEW LINE %ld XXXXX\n",GUI_DATA_L2);
      lnrAct = GUI_DATA_L2;
    }


  } else {
    printf(" .. key\n");
    // check for text-key
    if(isascii(GUI_DATA_I1)) {
      printf(" iascii-key %c mod=%d\n",GUI_DATA_I1,GUI_DATA_I2);
      // 
      return 0;
    }
      printf(" special-key %x mod=%d\n",GUI_DATA_I1,GUI_DATA_I2);
  }

}


//================================================================
  int cb_f1 (void *parent, void **data) {
//================================================================
// called when button is clicked ..

  static long  cpos=0, lNr=0;
  long         l1, l2, l3;
  char         *cp1, s1[256];

  cp1 = GUI_DATA_S1;
  printf("cb_f1 |%s|\n",cp1);


  //----------------------------------------------------------------
  if(!strcmp(cp1, "insTxt")) {
    printf(" insert text ..\n");
    strcpy (s1, "abc\ndef\nghi\n");
    l1 = strlen(s1);
    GUI_edi_Insert (&wEd, cpos, s1, l1);
  }


  //----------------------------------------------------------------
  if(!strcmp(cp1, "insFil")) {
    printf(" insert file ..\n");
    GUI_edi_InsFile (&wEd, "t2.dat");
  }


  //----------------------------------------------------------------
  if(!strcmp(cp1, "loaFil")) {
    printf(" copy file -> editor\n");
    // strcpy(s1,"Erste Zeile.\nZweite zeile.\n// EOF");
    // l1 = strlen(s1);
    UTX_tmp_file (cp1, &l1, "t1.dat");
    GUI_edi_Write (&wEd, cp1, &l1, 256);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "wriFil")) {
    printf(" get all chars > file\n");
    l3 = sizeof(s1);
    GUI_edi_Read (s1, &l3, 0L, -1L, &wEd);
      printf(" |%s|\n",s1);
    UTX_wrf_str ("t1.dat", s1);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "getPos")) {
    printf(" get cursor-position \n");
    cpos = GUI_edi_getCpos (&wEd);
    printf("%ld\n",cpos);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "setPos")) {
    printf(" set cursor-position \n");
    cpos += 5;
    GUI_edi_setCpos (&wEd, cpos);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "getLnr")) {
    printf(" get lineNr \n");
    lNr = GUI_edi_getLnr (&wEd);
    printf("%ld\n",lNr);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "setLnr")) {
    printf(" set lineNr \n");
    lNr += 2;
    GUI_edi_setLnr (&wEd, lNr);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "selReg")) {
    printf(" select region \n");
    l1 = cpos + 5;
    GUI_edi_sel__ (&wEd, cpos, l1);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "delReg")) {
    printf(" delete region \n");
    l1 = cpos + 5;
    GUI_edi_del (&wEd, cpos, l1);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "selGet")) {
    printf(" get selected region \n");
    l1 = 256;
    GUI_edi_sel_get (&l1, &l2, &wEd);
      printf(" sel %ld %ld\n",l1,l2);
    l3 = sizeof(s1);
    GUI_edi_Read (s1, &l3, l1, l2, &wEd);
      printf(" |%s|\n",s1);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "reaChr")) {
    printf(" get characters around cursor \n");
      printf(" %c\n",GUI_edi_RdChr(&wEd,-1));
      printf(" %c\n",GUI_edi_RdChr(&wEd,0));
      printf(" %c\n",GUI_edi_RdChr(&wEd,1));
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "reaLn")) {
    printf(" read active line\n");
    lNr = GUI_edi_getLnr (&wEd);
    printf("lNr = %ld\n",lNr);
    l1 = GUI_edi_RdLn (s1, 250, lNr, &wEd);
    printf("|%s|\n",s1);

  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "modLn")) {
    lNr = GUI_edi_getLnr (&wEd);
    printf(" modify line lNr %ld\n",lNr);
    GUI_edi_mod_ln (&wEd, lNr, "- this is the modified line ..");

  }


  //----------------------------------------------------------------
  if(!strcmp(cp1, "setLst")) {
    printf(" set cursot at start of line \n");
    l1 = GUI_edi_getLnr (&wEd);
    ++l1;
    cpos = GUI_edi_getLsta (&wEd, l1);
    GUI_edi_setCpos (&wEd, cpos);
  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "fsiz")) {
    // l1 = GUI_edi_getEof (&wEd);
    l3 = 0;
    GUI_edi_Read (s1, &l3, 0L, -1L, &wEd);
     printf(" %ld\n",l3);

  }

  //----------------------------------------------------------------
  if(!strcmp(cp1, "modif")) {
    printf(" %d\n",GUI_edi_getModif(&wEd));
    return 0;
  }


  //----------------------------------------------------------------
  if(!strcmp(cp1, "dummy-")) {
    printf(" do not set focus -> editor \n");
    return 0;
  }


  //----------------------------------------------------------------
  GUI_edi_Focus (&wEd); // else focus on button ..

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
      win0 = GUI_Win__ ("Gtk-ed-test1", GUI_main_quit, "");

      // get a hor. box in window
      boxv0 = GUI_box_v (&win0, "");
      boxv1 = GUI_box_h (&boxv0, "");

      // button in box
      GUI_button__ (&boxv1, "loaFil", cb_f1, (void*)"loaFil", "");
      GUI_button__ (&boxv1, "wriFil", cb_f1, (void*)"wriFil", "");
      GUI_button__ (&boxv1, "insTxt", cb_f1, (void*)"insTxt", "");
      GUI_button__ (&boxv1, "insFil", cb_f1, (void*)"insFil", "");
      GUI_button__ (&boxv1, "getPos", cb_f1, (void*)"getPos", "");
      GUI_button__ (&boxv1, "setPos", cb_f1, (void*)"setPos", "");
      GUI_button__ (&boxv1, "getLnr", cb_f1, (void*)"getLnr", "");
      GUI_button__ (&boxv1, "setLnr", cb_f1, (void*)"setLnr", "");
      GUI_button__ (&boxv1, "modLn",  cb_f1, (void*)"modLn",  "");

      boxv1 = GUI_box_h (&boxv0, "");

      GUI_button__ (&boxv1, "selReg", cb_f1, (void*)"selReg", "");
      GUI_button__ (&boxv1, "delReg", cb_f1, (void*)"delReg", "");
      GUI_button__ (&boxv1, "selGet", cb_f1, (void*)"selGet", "");
      GUI_button__ (&boxv1, "reaChr", cb_f1, (void*)"reaChr", "");
      GUI_button__ (&boxv1, "reaLn", cb_f1, (void*)"reaLn", "");
      GUI_button__ (&boxv1, "setLst", cb_f1, (void*)"setLst", "");
      GUI_button__ (&boxv1, "fsiz", cb_f1, (void*)"fsiz", "");
      GUI_button__ (&boxv1, "modif", cb_f1, (void*)"modif", "");
      GUI_button__ (&boxv1, "dummy-", cb_f1, (void*)"dummy-", "");

      wEd = GUI_edi__ (&boxv0, ed_CB, 0, "50,40");

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
