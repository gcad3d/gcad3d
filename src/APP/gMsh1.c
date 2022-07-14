/* ../APP/gMsh1.c

create GIS-mesh from inputfiles with point-data.

GUI-exe:
  GUI_gMsh1_gtk2 (Linux, GUI_gMsh1_gtk2_MS.exe)
  GUI_gMsh1.mak GUI_gMsh1.nmak
  ../gui_gtk2/GUI_gMsh1.c


Meshing-exe:
  gcad3d_gMsh   (Linux, MS: gcad3d_gMsh.exe)


*/




#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
// export this functions
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// import functions exported from the core (see gCAD3D.def)
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_os.h"               // OS_ ..
#include "../xa/mdl__.h"               // SIZMF*


// ext aus xa.c:
// extern  char      AP_mod_fnam[SIZMFNam];  // der Modelname




//=========================================================
  int gCad_main () {
//=========================================================
// user has selected this plugin; starting ...

  int     irc, i1;
  char    sDir[SIZMFTot], fnCmd[SIZFNam], fnMdl[SIZFNam], fnLog[SIZFNam],
          fnTmp[SIZFNam], s1[1200], s2[SIZFNam], *p1;
  FILE    *fpi, *fpo;



  // write to Main-Infowindow ..
  TX_Print("gCad_main of gMsh1.dll-V1.4");


  //----------------------------------------------------------------
  // check directory Data/gMsh; create if it not exists
  strcpy(s1, "Data/gMsh/");
  // get full filename
  irc = MDLFN_ffNam_fNam (sDir, s1);
    printf(" gMsh1-sDir |%s|\n",sDir);

  if(!OS_checkDirExist(sDir)) {
    TX_Error ("***** cannot create directory %s",sDir);
    goto L_exit;
  }

  sprintf(fnLog, "%sactMdl.log", OS_get_tmp_dir());


  //----------------------------------------------------------------
  // get commandFilename fnCmd out of file <tmpDir>gMsh1.txt
  // no file: set filename "unknown.gmsh1"

  // set fnTmp = filename info-file
  sprintf(fnTmp, "%sgMsh1.txt",AP_get_tmp_dir());
    printf(" gMsh1-fnTmp |%s|\n",fnTmp);

  // test if info-file exists
  irc = UTX_fgetLine (s1, SIZFNam, fnTmp, 1);
  if(irc < 0) {
    // no info-file exists; create one;
    // set sDir = default-direcory for commandfiles
    strcpy(s1, "Data/gMsh/");
    // get full filename
    irc = MDLFN_ffNam_fNam (sDir, s1);
      if(irc < 0) goto L_exit;

    if(!OS_checkDirExist(sDir)) {
      TX_Error ("***** cannot create directory %s",sDir);
      irc = -1;
      goto L_exit;
    }

    sprintf(fnCmd, "unknown.gmsh1");

    // create info-file
    if((fpo=fopen(fnTmp, "w")) == NULL) {
      TX_Error ("****** OPEN ERROR FILE %s",fnTmp);
      irc = -2;
      goto L_exit;
    }
    // 1.line = commandfilename
    fprintf(fpo,"%s%s\n",sDir,fnCmd);
    // 2.line = output of GUI.menu
    fprintf(fpo,"\n");
    fclose (fpo);

  // } else {
    // // separate directory-filename -> sDir - fnCmd
    // UTX_fnam1__ (sDir, fnCmd, s1);
  }
    // printf(" gMsh1-sDir |%s|\n",sDir);
    // printf(" gMsh1-fnCmd %d |%s|\n",irc,fnCmd);



  //----------------------------------------------------------------
  // do menu - 'GUI_gMsh1 "directory-dataFiles" "commandFilename"'

  // get s2 = filename executable GUI-menu
#ifdef _MSC_VER
  sprintf(s2, "%sGUI_gMsh1_gtk2_MS.exe",AP_get_bin_dir());
#else
  sprintf(s2, "%sGUI_gMsh1_gtk2",AP_get_bin_dir());
#endif

  irc = OS_checkFilExist (s2, 1);
  if(!irc) {TX_Print("**** executable %s does not exist",s2); goto L_exit;}

  // do GUI_gMsh1 = create commandFile
  // - in: s2 = exe-name; sDir = directory gMsh; 
  //       fnCmd = filename infofile 
  // - out: 2.line of file 
  // - makes testfile /tmp/debug.dat
#ifdef _MSC_VER
  // sDir is a directory, ending with '\'; for MS words must be included into  "..";
  // - to avoid removing the last '\ ' must add a blank or a \ !!!!
  // sprintf(s1,  "START \"\" /WAIT /B \"%s\" \"%s\\\" \"%s\"",s2,sDir,fnCmd);
  // sprintf(s1,  "CMD /C \"\"%s\" \"%s\\\" \"%s\"\"",s2,sDir,fnCmd);
  sprintf(s1,  "CMD /C \"\"%s\" \"%s\"\"",s2,fnTmp);
#else
  // sprintf(s1, "%s \"%s\" \"%s\"",s2,sDir,fnCmd);   // Linux
  sprintf(s1, "%s \"%s\"",s2,fnTmp);   // Linux
#endif
  // sprintf(s1, "%s %s %s",s2,sDir,fnCmd);
    printf(" gMsh1-do |%s|\n",s1);
  // irc = OS_sys1 (s2, 400, s1);
  irc = OS_system (s1);
  UTX_CleanCR (s2);
    printf(" gMsh1-menu-out |%s|\n",s2);

  // get output of gui GUI_gMsh1 out of line 2 of file fnTmp
  UTX_fgetLine (s2, SIZFNam, fnTmp, 2);
    printf("gMsh1-gui- output |%s|\n",s2);


  // check cancel
  if(!strlen(s2)) goto L_exit;

  // check for "__viewLog__"
  if(!strcmp(s2, "__viewHelp__")) {
    // display helpfile <docDir>/gMsh1.txt; see also ../myGIS1/README.txt
    sprintf(s2, "%sgMsh1.txt",AP_get_doc_dir());
    APP_edit (s2, 1);
    goto L_exit;
  }

  // check for "__viewLog__"
  if(!strcmp(s2, "__viewLog__")) {
    // display logfile <tmpDir>/gMsh.log
    APP_edit (fnLog, 1);
    goto L_exit;
  }

// TODO: get new commandFilename out of fnTmp




  // new commandFilename
  strcpy(fnCmd, s2);

  // check outfile
  irc = OS_checkFilExist (fnCmd, 1);
  if(!irc) {TX_Print("**** no commandfile created .."); goto L_exit;}

  // keep commandFilename fnCmd in file <tmpDir>gMsh1.fn
  UTX_fsavLine (fnCmd, fnTmp, SIZFNam, 1);


  //----------------------------------------------------------------
  // do mesh - get cad-model from executable gcad3d_gMsh

  // remove logFile
  OS_file_delete (fnLog);
    printf(" gMsh1-fnLog |%s|\n",fnLog);

  // set s2 = exe
#ifdef _MSC_VER
  // sprintf(s2, "%sgcad3d_gMsh.exe",AP_get_bin_dir());
  sprintf(s2,  "%sgcad3d_gMsh.exe",AP_get_bin_dir());
#else
  sprintf(s2, "%sgcad3d_gMsh",AP_get_bin_dir());
#endif
  irc = OS_checkFilExist (s2, 1);
  if(!irc) {TX_Print("**** executable %s does not exist",s2); goto L_exit;}

  // s2 = full-exeFileName
  // fnCmd = commandfileName
#ifdef _MSC_VER
  sprintf(s1, "\"%s\" \"%s\"",s2,fnCmd);
#else
  sprintf(s1, "%s %s",s2,fnCmd);
#endif
    printf(" gMsh1-do |%s|\n",s1);
  irc = OS_system (s1);


  //----------------------------------------------------------------
  // check / display errors

  // no logFile = cancel
  irc = OS_checkFilExist (fnLog, 0);
  if(!irc) goto L_exit;

  // find line "ENR <nrErrors> "
  irc = UTX_setup_get__ (s2, "ENR", fnLog);
  if(irc) return -1;
    printf(" ENR= |%s|\n",s2);

  i1 = strtol(s2, &p1, 10);
  if(p1 == s1) {TX_Print("**** gMsh1 E1"); goto L_exit;}
    // printf(" i1=%d\n",i1);

  // display with sysEd
  if(i1) {
    TX_Print ("***** %d errors - check errorLog",i1);
    OS_edit__ (fnLog);
  }


  //----------------------------------------------------------------
  // display outFile

  // get fnMdl = outfilename out of logfile
  sprintf(s2, "%sactMdl.log",OS_get_tmp_dir());
  
  // find  "=========== OUT " in logfile
  if ((fpi = fopen (s2, "r")) == NULL) {
    TX_Print("**** no logfile created ..");
    goto L_exit;
  }
  strcpy(fnMdl, "-");
  while (!feof (fpi)) {
    if (fgets (s2, 256, fpi) == NULL) break;
    p1 = strstr (s2, "=========== OUT ");
    if(p1) {       // 1234567890123456
      p1 += 16;
      strcpy(fnMdl, p1);
      UTX_CleanCR (fnMdl);
        // printf(" fnMdl |%s|\n",fnMdl);
      break;
    }
  }

  fclose (fpi);

  // check outfile
  irc = OS_checkFilExist (fnMdl, 1);
  if(!irc) {TX_Print("**** no meshfile created .."); goto L_exit;}

  // load model
  AP_Mod_load_fn (fnMdl, 0);

 
  //----------------------------------------------------------------
  // finish application
  L_exit:
    gCad_fini ();

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  printf("gCad_fini gis1\n");

  // write to Main-Infowindow ..
  TX_Print("...... gCad_fini gis1");

  AP_User_reset ();     // close application

  return 0;

}


