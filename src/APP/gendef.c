//                    gendef.c                    Reiter Franz.
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  gendef.c
\brief create Microsoft .def file from Linux executable. 
\code
=====================================================
List_functions_start:


List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Fuer Windows Datei gCAD3D.def erstellen.
NUR IN LINUX  !!  (input ist gCAD3D - das exe)

ACHTUNG: geht nicht mit einem gestrippten File !!!!


1) objdump -t ../gCAD3D > t1
2) aus t1 alle Funktionen und glob Variablen > t2
3) t2 sortieren, "EXPORTS" vorne ran = gCAD3D.def
   remove all funcs containing _tst_
4) t1 t2 loeschen, fertig.


----------------------------------------------------------------------
Compile:      NUR LINUX!

use with gendef.sh

cc -o gendef gendef.c && ./gendef
./gendef&&vi gCAD3D.def


----------------------------------------------------------------------
Format objdump ist:
32-bit  64bit
Spalte  Spalte
   9      17    1 char, type of function; (|l|g| |)
  15      23    "O .data"|"O .roda"
  17      25    ".text"|".bss"
  35      62    "main"|"_start"..

64-bit:
Spalte
  17          1 char, type of function; (|l|g| |)



*/




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int  pa32[] = { 9,15,17,35};
int  pa64[] = {17,23,25,60};



//=======================================================================
  int main (int paranz, char** argv) {
//=======================================================================

  int     ipos0, ipos1, iBit, *ipa;
  char    *lp,  cadExe[256], guiExe[256], line[256], wSiz[8], ftyp, *ps, *cp1;
  FILE    *fpi, *fpo;


  printf("--------- gendef ----------------\n");

  if(sizeof(void*) < 5) {
    printf(" 32-bit-System -\n");
    iBit = 32;
    ipa = pa32;
  } else {
    printf(" 64-bit-System -\n");
    iBit = 64;
    ipa = pa64;
  }



  cp1 = getenv ("HOME");
    printf(" home=%s\n",cp1);


  // cadExe = get filename of gcad3d-executable
  sprintf(cadExe, "%s/gCAD3D/binLinux%d/gCAD3D",cp1,iBit);


  // guiExe = get filename of gui-dll
  sprintf(guiExe, "%s/gCAD3D/binLinux%d/xa_gui.so",cp1,iBit);


  sprintf(wSiz, "%ld", sizeof(long) * 8);
    // printf(" wSiz |%s|\n",wSiz);

  sprintf(line, "objdump -t %s > t1",cadExe);
    printf(" system |%s|\n",line);
  system(line);
    // exit(0);

  sprintf(line, "objdump -t %s >> t1",guiExe);
    printf(" system |%s|\n",line);
  system(line);
    // exit(0);


  if((fpi=fopen("t1","r")) == NULL) {
    printf("****** OPEN ERROR FILE t1 **********\n");
    return -1;
  }


  if((fpo=fopen("t2","w")) == NULL) {
    printf("****** OPEN ERROR FILE t2 **********\n");
    return -1;
  }



  // // vorlesen bis nach "SYMBOL TABLE:"
  // L_next1:
  // lp = fgets(line, 250, fpi);
  // if(lp == NULL) goto L_fertig;
  // if(strncmp(line, "SYMBOL TABLE:",13)) goto L_next1;



  // get next line -> line
  L_nxt_ln:
  lp = fgets(line, 250, fpi);
  if(lp == NULL) goto L_fertig;
  if(strlen(line) < 20) goto L_nxt_ln;
  ftyp = line[ipa[0]];


  // wenn ab 17 nicht ".text" oder ".bss" steht: ignore ..
  if(!strncmp(&line[ipa[2]],   ".text",5)) goto L_check1;
  if(!strncmp(&line[ipa[2]],   ".bss", 4)) goto L_check1;
  if(!strncmp(&line[ipa[1]], "O .data",7)) goto L_check1;
  if(!strncmp(&line[ipa[1]], "O .roda",7)) goto L_check1;  // ReadOnly; const ..
  goto L_nxt_ln;


  // funktionstypen:
  L_check1:
  // g = globale Funktionen oder Variablen: diese checken !
  // l = lokale variable; skippen
  // ' ' = externe Funktionen (zB OpenGL, gtk clib ..); skippen.
  if(ftyp == 'l') goto L_nxt_ln;
  if(ftyp == ' ') goto L_nxt_ln;


  // das naechste Wort nach col 18 ist die size; find next blank
  ipos0 = ipa[2] + 1;
  ps = strpbrk(&line[ipos0], " \t\n");
  if(ps == NULL) goto L_nxt_ln;
  ++ps;
  // printf(" ps=|%s|\n",ps);


  // // skip all symbols mit siz == 0000000a  (leer; zB DB DL GL ..)
  // if(!strncmp(ps, "0000000a",  8)) goto L_nxt_ln;
  // schlecht: es wird auch OS_beep entsorgt! 



  // das naechste Wort nach col 32 ist der name
  ipos1 = ipa[3];
  while (line[ipos1]==' ') { ++ipos1; }


  // skip alle, die mit '_' beginnen (nur vom OS used !)
  if(line[ipos1] == '_') goto L_nxt_ln;


  // skip "_start"  und "__libc*"
  if(!strncmp(&line[ipos1], "main",  4)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "_start",6)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "__libc",6)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "__gmon",6)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "gtk_",4)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "gdk_",4)) goto L_nxt_ln;



  // nun die Testfunktionen ausfiltern:
  if(strstr(&line[ipos1], "_tst_") != NULL) goto L_nxt_ln;
  if(strstr(&line[ipos1], "_Test") != NULL) goto L_nxt_ln;



  // nun die geheimen ausfiltern:
  if(!strncmp(&line[ipos1], "os_l_", 5)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "cvApp_",6)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "UI_wc", 5)) goto L_nxt_ln;
  if(!strncmp(&line[ipos1], "wcg_",  4)) goto L_nxt_ln;



  // skip stdout@@GLIBC_2.0
  if(!strncmp(&line[ipos1], "stdout@@",  8)) goto L_nxt_ln;



  // OK, raus
  fprintf(fpo,"%s",&line[ipos1]);

  if(ftyp == 'g') goto L_nxt_ln;

  printf("%c %s ?????????????",ftyp,&line[ipos1]);

  goto L_nxt_ln;



  //========================================================
  L_fertig:
  fclose(fpo);
  fclose(fpi);

  // t2 sortieren, "EXPORTS" vorne ran = xa.def
  system("echo EXPORTS > gCAD3D.def");
  // system("sort t2 >> xa.def");
  system("sort t2 >> gCAD3D.def");


  // 4) t1 t2 loeschen, fertig.
  // system("rm t1 t2");


  printf(">>>>>>>>>> file gCAD3D.def created <<<<<<<<<<<<<<<<\n");
  system("ls -l gCAD3D.def");



  L_exit:
  return 0;

}


//============================= EOF ================================
