/* tst_gui1.c           C-KIT-GUI-GTK - Demoprog.

---------------------------------------------------------
TODO:


---------------------------------------------------------
Modifications:
2012-04-24 neu erstellt. RF.


----------------------------------
*//*!
\file ../gui/tst_gui1.c
\ingroup grp_exa_gui
\code
*/
#ifdef globTag
 void TST_GUI1(){}
#endif
/*
----------------------------------


make run -f tst_gui1.mak "OS=Linux32"

export LD_LIBRARY_PATH=~/binLinux32 && ~/binLinux32/a.out


*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/ut_umem.h"                // Memspc MemObj UME_*

#include "../gui/gui__.h"             // GUI_*


// vars
static MemObj   but0, ent1, wsl1;
static MemObj   win0, win1, win2 = GUI_OBJ_NEW, win3 = GUI_OBJ_NEW;
static MemObj   lbl0, lbl1, lbl2, rb2, wmcb1, wml, wgl, wnb, wtb;


// protos
  int UI_butt1_cb (MemObj *mo, void **data);
  int cb_list2    (MemObj *mo, void **data);
  int pop_cp      (MemObj *mo, void **data);
  int tst_gl_draw ();
  int tst_gl_move ();
  int tst_gl_button ();
  int tst_gl_key ();



//================================================================
  int UI_cbp1 (char *fnamC, char *parentTxt) {
//================================================================
/// \code
/// callback parent-sel/create childs-file from selected parent
/// fnamC   filename (write childs of parent <parentTxt> into this file)
/// \endcode

  int  i1;
  FILE *fpo;


  printf("UI_cbp1 |%s|%s|\n",fnamC,parentTxt);

  if((fpo=fopen(fnamC,"w"))) {
    for(i1=0; i1<10; ++i1) fprintf(fpo,"%s%d\n",parentTxt,i1);
    fclose(fpo);
  }


  return 0;

}


//================================================================
  int UI_cbp2 (char *parentTxt, char *childtxt) {
//================================================================
// callback child-selection

  printf("UI_cbp2 |%s|%s|\n",parentTxt,childtxt);


  return 0;

}


 

//================================================================
  int mywin2 (MemObj *mo, void **data) {
//================================================================
 
  int     iTyp;
  static  MemObj ent1;
  MemObj  box1, box2;
  void    *wo;


  iTyp = GUI_OBJ_TYP (mo);
  printf("mywin2 ev=%d typ=%d\n",GUI_DATA_EVENT,iTyp);


  if(GUI_DATA_EVENT == TYP_EventEnter) {
    if(iTyp == TYP_GUI_Entry) {
      printf(" enter entry\n");
    }
    if(iTyp == TYP_GUI_Notebook) {
      printf(" enter notebook-page %d\n",GUI_DATA_I1);
    }
    return 0;
  }

  if(GUI_DATA_EVENT == TYP_EventMove) {
    printf(" slider-move %lf\n",GUI_DATA_D1);
    return 0;
  }


  if(GUI_DATA_EVENT == UI_FuncOK) {
    printf(" GUI_DialogYN-YES\n");
  } else if(GUI_DATA_EVENT == UI_FuncCancel) {
    printf(" GUI_DialogYN-NO\n");
  }




  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == UI_FuncInit) {
    if(GUI_OBJ_IS_VALID(&win2)) return -1;


    win2 = GUI_Win__ ("mywin2", mywin2, "");
    // box1 = GUI_box__ (&win2, 1, 0, 0);


    // notebook
    wnb = GUI_notebook__ (&win2, mywin2);
    box1 = GUI_notebook_add (&wnb, "Page1");
    box2 = GUI_notebook_add (&wnb, "Page2");


    GUI_label__ (&box1, "Label in win2..", "");
    GUI_button__ (&box1, " Butt2 ", mywin2, "but3", "");
    GUI_button_img (&box1, "menOn.xpm", mywin2, "buti", "");
    GUI_ckbutt__ (&box1, "ckbutt", 0, mywin2, "ckb1", "");

    ent1 = GUI_entry__ (&box1, NULL, "preset", mywin2, "e1", "e");
    // ent1 = GUI_entry__ (&box1, "entry-", "preset", mywin2, "e1", 0, 0);

  // GUI_Slider__ --------------------
    wsl1 = GUI_Slider__ (&box1, 0, mywin2, 0., 1., 0., "e");


    GUI_Win_go (&win2);

    GUI_Win_up (NULL, &win2, 0);
    // GUI_Win_up (NULL, &win2, 1);

    return 0;
  }


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventExit) {
    printf(" exiting mywin2 ..\n");
    win2 = GUI_OBJ_INVALID ();
    return 0;
  }


  //----------------------------------------------------------------
  // button-release
  if(GUI_DATA_EVENT == TYP_EventRelease) {

    if(iTyp == TYP_GUI_Entry) {
      printf(" entry=|%s|\n",GUI_entry_get(&ent1));
      return 0;
    }
  }


  //----------------------------------------------------------------
  // buttonpress
  if(GUI_DATA_EVENT == TYP_EventPress) {
    printf(" data=|%s|\n",GUI_DATA_S1);

    // slider: skip it ?
    if(iTyp == TYP_GUI_Slider) return 0;


    if(!strcmp(GUI_DATA_S1, "ckb1")) {
        printf(" CKB1: %d\n",GUI_ckbutt_get(mo));
      return 0;
    }

    printf(" cpos=%d\n",GUI_entry_pos_get(&ent1));
    // GUI_entry_pos_set (0, &ent1);
    return 0;
  }


  return 0;
}


//================================================================
  int mywin3 (MemObj *mo, void **data) {
//================================================================

  int    iTyp, iSrc;
  static MemObj  omb, om1, om2;
  static MemObj  wmsg;
  static MemObj  box1, box2, ent1, wm1, wm2, wm3;
  MemObj wTmp1, wTmp2, oWin;
  void   *wo;
  char   *ppLst[]={"pp-opt 1","pp-opt 2","pp-opt 3",NULL};
  char   *omLst[]={"om-opt 1","om-opt 2","om-opt 3",NULL};
  char   s1[204], s2[204], *userData;
  Obj_Unknown *go;


  printf("mywin3 %d\n",GUI_DATA_EVENT); // see TYP_Event* ../gui/gui_types.h
  // printf(" mywin3=%p\n",mywin3);
  // UMB_dump__ ();
  // GUI_obj_dump__ ();


  if(GUI_DATA_EVENT == TYP_EventExit) {
    printf(" exiting mywin3 ..\n");
    win3 = GUI_OBJ_INVALID ();
    return 0;
  }


  if(GUI_DATA_EVENT == UI_FuncInit) {
      printf(" UI_FuncInit\n");

    if(GUI_OBJ_IS_VALID(&win3)) return -1;

    oWin = GUI_Win__ ("mywin3", mywin3, "");
    box1 = GUI_box_v (&oWin, "");



    //----------------------------------------------------------------
    // menu
    wm1 = GUI_menu__ (&box1, NULL, 0);            // the toolbar
    wm2 = GUI_menu__ (&wm1, "File ", 0);

    GUI_menu_entry (&wm2, "New",   mywin3,  (void*)"new");
    GUI_menu_entry (&wm2, "Open",  mywin3,  (void*)"open");

    wm3 = GUI_menu__ (&wm2, "Save as ", 0);
    GUI_menu_entry  (&wm3, "dxf", mywin3,      (void*)"saveDxf");
    GUI_menu_entry  (&wm3, "svg", mywin3,      (void*)"saveSvg");
    GUI_menu_entry  (&wm3, "igs", mywin3,      (void*)"saveIgs");

    GUI_menu_entry  (&wm2, "---", NULL, NULL);  // menu-separatorline

    wmcb1 = GUI_menu_checkbox__ (&wm2, "menu-checkbox", 0, mywin3,  (void*)"wmcb1");

    wm2 = GUI_menu__     (&wm1, "Lang ", 'r');
          GUI_menu_radiobutt__ (&wm2, "de", 0, mywin3, "l-de");
    wml = GUI_menu_radiobutt__ (&wm2, "en", 1, mywin3, "l-en");
          GUI_menu_radiobutt__ (&wm2, "es", 1, mywin3, "l-es");
          GUI_menu_radiobutt__ (&wm2, "it", 1, mywin3, "l-it");
    GUI_menu_radiobutt_set (&wml);               // preset en



    //----------------------------------------------------------------
    wTmp1 = GUI_frame__ (&box1, "RB's", 4);
    wTmp2 = GUI_box_h (&wTmp1, "");
    // radiobuttons
          GUI_radiobutt__ (&wTmp2, "rb1", 0, mywin3, (void*)"rb1", "");
    rb2 = GUI_radiobutt__ (&wTmp2, "rb2", 1, mywin3, (void*)"rb2", "");


    //----------------------------------------------------------------
    // popup-menu
    // GUI_popup__ (ppLst, dataLst, tipLst, mywin3);


    //----------------------------------------------------------------
    // option-menu
    omb = GUI_box_h (&box1, "");
    om1 = GUI_optmen__ (&omb, "men1", omLst, NULL, mywin3, "12");
    om2 = GUI_optmen__ (&omb, "men1", omLst, omLst, mywin3, "12");


    //----------------------------------------------------------------
    GUI_label__ (&box1, "Label in win3..", "");
    GUI_button__ (&box1, "active", mywin3, "but2", "");
    GUI_button__ (&box1, " Exit ", mywin3, "but3", "");

    // ent1 = GUI_entry__ (&box1, NULL, "preset", mywin2, "e1", 0, 0);
    // ent1 = GUI_entry__ (&box1, "entry-", "preset", mywin2, "e1", 0, 0);

    //----------------------------------------------------------------
    wmsg = GUI_msgwin__ (&box1, "0,3");


    //----------------------------------------------------------------
    GUI_Win_go (&oWin);
    GUI_Win_up (NULL, &oWin, 0);
    win3 = oWin;  // now window is up = MemObj is valid.

    return 0;
  }


  //================================================================
  iTyp = GUI_OBJ_TYP (mo);
  iSrc = GUI_OBJ_SRC (mo);
    printf(" typ=%d src=%d\n",iTyp, iSrc);


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventPress) {

    if(iTyp == TYP_GUI_MenuItem) {
      if(!strcmp(GUI_DATA_S1, "wmcb1")) {
        printf("  press menu_checkbox |%s|\n",GUI_DATA_S1);

      } else if(!strcmp("open", GUI_DATA_S1)) {
        strcpy(s2, ".");
        GUI_file_open__ (s1, 200, s2, 200, NULL, NULL, "open", "*.c");
        printf(" open |%s|%s|\n",s1,s2);

      } else if(!strcmp("saveDxf", GUI_DATA_S1)) {
        strcpy(s2, ".");
        // GUI_file_save__ (s1, 200, s2, 200, NULL, NULL, "open", "*.c");
        // printf(" save |%s|%s|\n",s1,s2);

      } else if(!strncmp(GUI_DATA_S1, "l-", 2)) {
          printf(" change language -> |%s|\n",GUI_DATA_S1);
        if(GUI_OBJ_IS_VALID(&win3)) {
          GUI_msgwin_prt (&wmsg, " change language ");
        }
      }
      return 0;
    }

    // check for optmen-callback
    if(iTyp == TYP_GUI_OptMen) {
      printf(" sel lNr=%d txt=|%s|\n",GUI_DATA_I1,GUI_DATA_S2);
      if(!GUI_obj_cmp(mo, &om1)) {      // is parent om1 ?
        GUI_optmen_set (&om1, 1, NULL); // hilite/active
        GUI_optmen_set (&om2, 0, NULL); // unhilite
      } else {
        GUI_optmen_set (&om1, 0, NULL); // hilite/active
        GUI_optmen_set (&om2, 1, NULL); // unhilite
      }

      return 0;
    } 


    printf(" data=|%s|\n",GUI_DATA_S1);

    if(!strcmp(GUI_DATA_S1, "but2")) {
      if(GUI_set_enable(&omb, 2) == 0) {
        GUI_set_enable (&omb, 1);
      } else {
        GUI_set_enable (&omb, 0);
      }
    }


    if(!strcmp(GUI_DATA_S1, "but3")) {
        printf(" TYP_EventPress\n");
      // printf(" |%s|\n",GUI_entry_get(&ent1));
      printf(" exit\n");
      GUI_Win_kill (&win3);
    }

    if(!strcmp(GUI_DATA_S1, "rb1")) {
      printf(" rb1=%d\n",GUI_radiobutt_get (mo));
      GUI_msgwin_prt (&wmsg, " select rb1 ");
    }

    return 0;
  }

  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventRelease) {
    if(iTyp == TYP_GUI_MenuItem) {
      if(!strcmp(GUI_DATA_S1, "wmcb1")) {
        printf("  release menu_checkbox |%s|\n",GUI_DATA_S1);
      } else {
        printf("  release menu_radiobutt |%s|\n",GUI_DATA_S1);
      }
    }
  }

  return 0;
}


//================================================================
  int mywin4 (MemObj *mo, void **data) {
//================================================================

  int             i1, iTyp, iSrc;
  static MemObj   wt1;
  static TreeNode actNd;
  MemObj          win1, box0, box1;
  static TreeNode node1, node2, node3;

  char *icoTab[]={
    "value.png",  "vector.png", "point.png",  "line.png",   "circle.png",  // 0-4
    "curve.png",  "plane.png",  "surface.png","body.png",   "image.png",   // 5-9
    "part.png",   "natMdl.png", "extMdl.png", "libPart.png","binDat.png",  // 10-
    "link.png",   NULL};


  printf("mywin4 %d\n",GUI_DATA_EVENT);



  //================================================================
  if(GUI_DATA_EVENT == UI_FuncInit) {
      printf(" UI_FuncInit\n");

  printf("Ico_init \n");

  GUI_Ico_init (icoTab); 



    win1 = GUI_Win__ ("mywin4", NULL, "");
    box0 = GUI_box_v (&win1, "");
    box1 = GUI_box_h (&box0, "");
    GUI_button__ (&box1, "exp",  mywin4, "exp", "");
    GUI_button__ (&box1, "clr",  mywin4, "clr", "");
    GUI_button__ (&box1, "dim",  mywin4, "dim", "");
    GUI_button__ (&box1, "hil",  mywin4, "hil", "");
    GUI_button__ (&box1, "ChSet",  mywin4, "ChSet", "");
    GUI_button__ (&box1, "selRow",  mywin4, "selRow", "");
    GUI_button__ (&box1, "deSel",  mywin4, "deSel", "");


    wt1 = GUI_tree1__ (&box0, mywin4, "30,40");


    // populate tree ..
    GUI_tree1_row_add (&node1, &wt1, NULL,   -1, "abc", 1);
    GUI_tree1_row_add (&node2, &wt1, &node1, -1, "def", 1);
    GUI_tree1_row_add (&node1, &wt1, NULL,   -1, "ghi", 1);
    GUI_tree1_row_add (&node2, &wt1, &node1, -1, "jkl", 1);
    GUI_tree1_row_add (&node2, &wt1, &node1, -1, "mno", 1);




    //----------------------------------------------------------------
    GUI_Win_go (&win1);
    GUI_Win_up (NULL, &win1, 1);

    return 0;
  }


  //================================================================
  iTyp = GUI_OBJ_TYP (mo);
  iSrc = GUI_OBJ_SRC (mo);
    printf(" mywin4 typ=%d src=%d\n",iTyp, iSrc);


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventPress) {

    if(iTyp == TYP_GUI_Tree) {  // tree-selection
      actNd = *((TreeNode*)data[2]);
      // GUI_tree1_row_set (&wt1, &actNd, -1, NULL, 2);
      printf(" sel.Row |%s|\n", GUI_DATA_S3);
      printf(" iconNr = %d\n", GUI_DATA_I4);
      printf(" state  = %d\n", GUI_DATA_I5);
      printf(" rowLevel=%d\n", GUI_tree1_lev(&wt1, &actNd));
      printf(" childNr=%d\n", GUI_tree1_childNr(&wt1, &actNd));
      return 0;
    }


    printf(" data=|%s|\n",GUI_DATA_S1);

    if(!strcmp(GUI_DATA_S1, "exp")) {
      GUI_tree1_expand__ (&wt1, &actNd);
    }
    if(!strcmp(GUI_DATA_S1, "clr")) {
      // GUI_tree1_clear (&wt1);
      // GUI_tree1_remove__ (&wt1, &actNd);
      GUI_tree1_unselect_all (&wt1);
    }
    if(!strcmp(GUI_DATA_S1, "dim")) {
      // deactivate last selected row 
      GUI_tree1_row_set (&wt1, &actNd, -1, NULL, -1);
    }
    if(!strcmp(GUI_DATA_S1, "std")) {
      // deactivate last selected row 
      GUI_tree1_row_set (&wt1, &actNd, -1, NULL, -1);
    }
    if(!strcmp(GUI_DATA_S1, "hil")) {
      // hilited
      GUI_tree1_row_set (&wt1, &actNd, -1, NULL, 1);
    }
    if(!strcmp(GUI_DATA_S1, "selRow")) {
      GUI_tree1_set_selMode (&wt1, 1);  // do not report following selections
      // find string with text "jkl" in group "ghi"
      i1 = GUI_tree1_iter_string (&node1, "ghi", NULL, &wt1);
      if(i1 < 0) return -1;
      i1 = GUI_tree1_iter_string (&node2, "jkl", &node1, &wt1);
      if(i1 < 0) return -1;
      // select this row
      GUI_tree1_row_set (&wt1, &node2, -1, NULL, 2);
      GUI_tree1_set_selMode (&wt1, 0);  // report all following selections
    }
    if(!strcmp(GUI_DATA_S1, "deSel")) {
      // unselect row from selRow
      GUI_tree1_row_set (&wt1, &node2, -1, NULL, -2);
    }
    if(!strcmp(GUI_DATA_S1, "ChSet")) {
      // GUI_tree1_childs_set (&wt1, &actNd, -1);
      GUI_tree1_childs_remove (&wt1, &actNd);
    }

  }


  return 0;

}


//================================================================
  int f_e1_cb (MemObj *mo, void **data) {
//================================================================
// 303

  printf("f_e1_cb event=%d data=|%s| key=%d mod=%d\n",
           GUI_DATA_EVENT,    // TYP_EventExit
           GUI_DATA_S1,       // user-provided data
           GUI_DATA_I2,       // key
           GUI_DATA_I3);      // mod


  if(GUI_DATA_EVENT == TYP_EventPress) {
    if(GUI_DATA_I2 == 'a') return 1;  // skip handling this key
  }

  if(GUI_DATA_EVENT != TYP_EventRelease) return 0;

  printf(" after modif: |%s|\n",GUI_entry_get(mo));
  // printf(" |%s|\n",GUI_entry_get(&ent1));


  // GUI_button_mod (&but0, " new butCapt");
  // GUI_label_mod (&lbl0, " bla bla ..");

  return 0;

}


//================================================================
  int pop_cp (MemObj *mo, void **data) {
//================================================================

  // printf("pop_cp event=%d\n",
           // GUI_DATA_EVENT);

  if(GUI_DATA_EVENT == TYP_EventPress) {
    printf(" select %d |%s|\n",GUI_DATA_I1, GUI_DATA_S2);
  }
  if(GUI_DATA_EVENT == TYP_EventEnter) printf(" enter %d\n",GUI_DATA_I1);
  if(GUI_DATA_EVENT == TYP_EventLeave) printf(" leave %d\n",GUI_DATA_I1);

  return 0;

}


//================================================================
  int UI_butt1_cb (MemObj *mo, void **data) {
//================================================================

  int       irc, i1, i2, i3, iTyp;
  char      s1[256], s2[64], *ps;
  char      *buttons[]={"OK","Cancel",NULL};
  char      *popTxt[]={"txt pop1", "txt pop2", "txt pop3", NULL};
  char      *popTip[]={"tip pop1", "tip pop2", "tip pop3", NULL};
  static char *popDat[]={"pop1", "pop2", "pop3", NULL};
  char      *titLst[]={"Directory","Filename"};
  void      *pa[4];


  iTyp = GUI_OBJ_TYP (mo);
  printf("UI_butt1_cb event=%d typ=%d\n",
           GUI_DATA_EVENT, iTyp);


  if(iTyp == TYP_GUI_List) {
    if(GUI_DATA_EVENT == TYP_EventPress) {
      printf(" ListSel %d %d |%s|\n",GUI_DATA_I1,GUI_DATA_I2,GUI_DATA_S3);
      return 0;
    }
    if(GUI_DATA_EVENT == TYP_EventExit) {
      printf(" cancel selection-list\n");
      return 0;
    }
  }


  printf("UI_butt1_cb event=%d data=|%s|\n",
           GUI_DATA_EVENT,    // TYP_EventExit
           GUI_DATA_S1);      // user-provided data


  if(!strcmp("but1", GUI_DATA_S1)) {
    // GUI_MsgBox ( " but 1 pressed ..");
  // GUI_DialogEntry --------------------
    // sprintf(s1, " old text ..");
    // sprintf(s2, "enter new text for joint ");
    // i1 = GUI_DialogEntry (s2, s1, sizeof(s1), buttons, 2);
      // printf(" returns %d\n",i1);
  // GUI_DialogYN --------------------
    // GUI_DialogYN ("test\n GUI_DialogYN", mywin2);
  // GUI_Dialog_2b --------------------
    // i1 = GUI_Dialog_2b ("label", "but1", "but2");
      // printf(" returns %d\n",i1);
  // GUI_Dialog_e2b --------------------
    // sprintf(s1, " old text ..");
    // sprintf(s2, "enter new text for joint ");
    // i1 = GUI_Dialog_e2b ("label", s1, 12, "but1", "but2");
      // printf(" returns %d\n",i1);
  // GUI_Color_select --------------------
    // { ColRGB col1;
      // irc = GUI_Color_select (&i1, &i2, &i3);
      // if(irc >= 0) Col_set__ (&col1, i1>>256, i2>>256, i3>>256); }
  //
    // GUI_Slider_set (&wsl1, 0.5);
    // printf(" slider=%lf\n",GUI_Slider_get (&wsl1));
    GUI_label_htm_mod (&lbl2, "<span weight=\"bold\">Text:</span>");
    // GUI_radiobutt_set (&rb2);
    // printf(" lang=%d\n",GUI_menu_radiobutt_get(&wml));



    // test if mywin2 is active; if yes: switch back to page 0.
    if(GUI_OBJ_IS_VALID(&win2)) {
      printf(" act.notebook-page = %d\n",GUI_notebook_get(&wnb));
      GUI_notebook_set (&wnb, 0);
    } else {
      printf(" - win2 not active\n");
    }


    GUI_set_show (&wtb, 1);  // show toolbar  (hide with "tb off")



  } else if(!strcmp("but3", GUI_DATA_S1)) {
    mywin2 (NULL, GUI_SETDAT_E (UI_FuncInit));


  } else if(!strcmp("but4", GUI_DATA_S1)) {
    mywin3 (NULL, GUI_SETDAT_E (UI_FuncInit));


  } else if(!strcmp("but5", GUI_DATA_S1)) {
    GUI_popup__ (popTxt, popTip, 0, pop_cp, (void**)popDat);


  } else if(!strcmp("but6", GUI_DATA_S1)) {
      // test Liste 1-columns
    GUI_list1_dlg__ (NULL, "List 1 column",
                     "list1.dat",
                     "1", titLst, cb_list2, "60,20");

  // irc = GUI_list1_dlg_w (s1, 200, NULL, "List 1 column",
                         // "/mnt/serv1/Devel/dev/gCAD3D/gui_gtk/list1.dat",
                         // "1", titLst, 60, 20);
    // printf(" sel %d |%s|\n",irc,s1);
/*
  GUI_list1_h2__ ("tit par",
                  "tit chd",
                  "change catalog",        // caption button
                  "abc",                   // active parent
                  "/mnt/serv1/Devel/dev/gCAD3D/xa/t1",  // filename parents
                  UI_cbp1,         // callback parent-sel/create childs-file
                  UI_cbp2,         // callback child-selection
                  -300,-300);
*/
  // test Liste 2-columns
  // GUI_list1_dlg__ (NULL, "List 2 columns", filNam, "2",
                  // titLst, cb_list2cb, 60, 20);


  } else if(!strcmp("but7", GUI_DATA_S1)) {
    GUI_gl_block (&wgl, 0);


  } else if(!strcmp("but8", GUI_DATA_S1)) {
    i1 = GUI_set_show (&wtb, 0);         // hide toolbar
      printf(" state toolbar = %d\n",i1);


  } else if(!strcmp("but9", GUI_DATA_S1)) {
    // tree1
    mywin4 (NULL, GUI_SETDAT_E (UI_FuncInit));



  } else {    // press "Butt2"
    GUI_button_mod (&but0, " new butCapt");
    GUI_button_styl (&but0, 2);
    GUI_label_mod (&lbl0, " bla bla ..");
    GUI_label_styl (&lbl0, 1);
  }

  return 0;

}
  

//================================================================
  int cb_list2 (MemObj *mo, void **data) {
//================================================================
// callback of "List2"

  // exit with doubleclick

  if(GUI_DATA_EVENT == TYP_EventExit) {
    printf(" exit List2\n");
    return 0;
  }

  printf(" mouseButtNr=%d selColNr=%d\n",GUI_DATA_I1,GUI_DATA_I2);
  printf(" txt sel |%s|\n",GUI_DATA_S3);


  if(GUI_DATA_EVENT == TYP_EventPress) {
    // close & exit
    GUI_list1_dlg_del ();
  }

  return 0;

}


//================================================================
  int f_cb_exit (MemObj *mo, void **data) {
//================================================================

  printf("f_cb_exit event=%d\n",
           GUI_DATA_EVENT);   // TYP_EventExit

  gCad_fini ();

  return 0;

}


//================================================================
  int f_cb_key (MemObj *mo, void **data) {
//================================================================

  int  iKey, iMod;

  printf("f_cb_key event=%d key=%d  modif=%d\n",
           GUI_DATA_EVENT,    // event:    TYP_EventPress|TYP_EventRelease
           GUI_DATA_I1,       // key:      ascii-value of key
           GUI_DATA_I2);      // modifier: &1=shift; &4=ctrl; &8=alt.
  // 302=press, 303=release

  if(GUI_DATA_EVENT == TYP_EventRelease) return 0;     // skip keyRelease

  return 0;
}


//================================================================
  int f_cb_but (void *parent, void **data) {
//================================================================

  int  iKey, iMod;

  printf("f_cb_but event=%d button=%d\n",
           GUI_DATA_EVENT,    // TYP_EventPress|TYP_EventRelease
           GUI_DATA_I1);      // 1=left MB; 2=middle MB; 3=right MB; 4=wheel

  return 0;
}


//================================================================
  int tst_gui () {
//================================================================


  int    irc, isiz;
  char   filNam[256], dirNam[256];
  char   *titLst[]={"Directory","Filename"};
  char   *buttons[]={"YES","Cancel",NULL};

  MemObj   box0, box1, box2;


  TX_Print("tst_gui() .. 9");
  printf("tst_gui ================================ \n");


  win0 = GUI_Win__ ("testWin0", f_cb_exit, "");
  box0 = GUI_box_v (&win0, "");

  box1 = GUI_box_h (&box0, "");
    GUI_label__ (&box1, "Label 0", "");
    lbl0 = GUI_label__ (&box1, "Label..", "");

  box1 = GUI_box_h (&box0, "");
    GUI_img__ (&box1, "unInstall.xpm", "");
    lbl2 = GUI_label_htm__ (&box1, "<span size=\"x-small\">Text:</span>", "");

  box1 = GUI_box_h (&box0, "");
    GUI_button__ (&box1, " Butt1 ", UI_butt1_cb, "but1", "e");
      GUI_Tip ("restore toolbar on bottom");

    but0 = GUI_button__ (&box1, " Butt2 ", UI_butt1_cb, "but2", "e");
      GUI_Tip ("update and copy objects to Model (Ctrl + right Mousebutton)");


  box1 = GUI_box_h (&box0, "");
    GUI_button__ (&box1, " window2 ", UI_butt1_cb, "but3", "");
    GUI_button__ (&box1, " window3 ", UI_butt1_cb, "but4", "");



  // GUI_Win_ev_key (&win0, f_cb_key);
  // GUI_Win_ev_button (&win0, f_cb_but);


  ent1 = GUI_entry__ (&box0, " entry-", "preset", f_e1_cb, "e1", "");


  //----------------------------------------------------------------
  GUI_box_paned__ (&box1, &box2, &box0, 0, 0, 100);

  GUI_list1__ (&box1, "list1.dat",
               "1", NULL, UI_butt1_cb, "e");

  // setUp OpenGL-window
  wgl = GUI_gl__ (&box2, tst_gl_draw, "-200e,-200e");

  // connect the mouse-move events
  GUI_gl_ev_move (&wgl, tst_gl_move);

  // connect the mouse-button events
  GUI_gl_ev_button (&wgl, tst_gl_button);

  // connect the keyboard-events
  GUI_gl_ev_key (&wgl, tst_gl_key);




  //----------------------------------------------------------------
  wtb = GUI_toolbox__ (&box0);
  box2 = GUI_box_h (&wtb, "");
  GUI_button__ (&box2, " popup ", UI_butt1_cb, "but5", "");
  GUI_button__ (&box2, " List2 ", UI_butt1_cb, "but6", "");
  GUI_button__ (&box2, "tree1", UI_butt1_cb, "but9", "");
  GUI_button__ (&box2, "unblock GL", UI_butt1_cb, "but7", "");
  GUI_Tip ("block GL with key b in gl-window; change cursor with key c");
  GUI_button__ (&box2, "tb off", UI_butt1_cb, "but8", "");


  //----------------------------------------------------------------
  GUI_Win_go (&win0);
  GUI_Win_up (NULL, &win0, 0);



  return 0;

}


//================================================================
//================================================================
// #ifdef OFFLINE
//================================================================
//================================================================

int main() { GUI_Init__ (""); tst_gui (); return 0; }

// #endif

// EOF
