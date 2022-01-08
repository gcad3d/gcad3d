/* example Postprocessor for gCAD3D.                    2003-10-02


-----------------------------------------------------
Modifications:
2013-04-02 Extract from DemoProstProc. RF.


Build:
make -f ../prc/cut1/G-Code.mak


-----------------------------------------------------
*/



// #include "MS_Def1.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "pp_ut.h"


  static  char   inFilnam[256];
  static  char   outFilnam[400];

  FILE   *fpLog = NULL;


/// FilenamedelimiterChar
#ifdef _MSC_VER
#define  fnam_del '\\'
#define  fnam_del_s "\\"
#else
#define  fnam_del '/'
#define  fnam_del_s "/"
#endif





//=========================================================
  int main (int parNr, char* argv[])  {
//=========================================================

  char    iRpd=0;
  int     iRnr=0;
  FILE    *fpi, *fpo;
  char    *p1, cbuf[512];


  // 1.par = inputfilename
  if(parNr < 2) {
    TX_Error ("G-Code: 1.Parameter must be inputfilename");
    return -1;
  }
    printf(" a0=|%s|\n",argv[0]);
    printf(" a1=|%s|\n",argv[1]);


  strcpy(inFilnam, argv[1]);
  if((fpi=fopen(inFilnam,"r")) == NULL) {
    TX_Error("G-Code: OPEN ERROR FILE %s **********\n",inFilnam);
    return -1;
  }

  // get directory of inputfile
  p1 = strrchr (inFilnam, fnam_del);
  if(p1) {
    ++p1;
    *p1 = '\0';
  } else {
    TX_Error("G-Code: E001");
    return -1;
  }


  sprintf(outFilnam, "%snc.log",inFilnam);
    printf(" logFilnam |%s|\n",outFilnam);
  if((fpLog=fopen(outFilnam,"w")) == NULL) {
    TX_Error("G-Code: OPEN ERROR FILE %s **********\n",outFilnam);
    return -1;
  }


  sprintf(outFilnam, "%snc.iso",inFilnam);
    printf(" outFilnam |%s|\n",outFilnam);
  if((fpo=fopen(outFilnam,"w")) == NULL) {
    TX_Error("G-Code: OPEN ERROR FILE %s **********\n",outFilnam);
    return -1;
  }

  TX_Print("G-Code: outfile is %s",outFilnam);


  while (!feof (fpi)) {
    if (fgets (cbuf, 250, fpi) == NULL) break;
    UTX_CleanCR (cbuf);                            // remove foll CR,LF ..
      // printf(" in:|%s|\n",cbuf);


    //----------------------------------------------------------------
    if(!strncmp(cbuf, "FROM ", 5)) {
      fprintf(fpo, "G92 %s\n", &cbuf[5]);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "RP", 2)) {
      iRpd = 1;
      continue;


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "G1 ", 3)) {
      if(iRpd == 1) {
        fprintf(fpo, "G0 %s\n", &cbuf[3]);
        iRpd = 0;
      } else {
        fprintf(fpo, "%s\n", cbuf);
      }


    //----------------------------------------------------------------
    // G2=CW; G3=CCW  I J K
    } else if((!strncmp(cbuf, "G2 ", 3))  ||
       (!strncmp(cbuf, "G3 ", 3)))     {
      fprintf(fpo, "%s\n", cbuf);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "TL ", 3)) {
      fprintf(fpo, "T%s\n", &cbuf[3]);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "FED ", 4)) {
      fprintf(fpo, "F%s\n", &cbuf[4]);


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "STOP", 4)) {
      if(strlen(cbuf) > 5) {
        fprintf(fpo, "M0%s\n", &cbuf[5]);
      } else {
        fprintf(fpo, "M00\n");
      }


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "COOL", 4)) {
      if(!strcmp(&cbuf[5], "OFF") > 5) {
        fprintf(fpo, "M09\n");
      } else {
        fprintf(fpo, "M07\n");
      }


    //----------------------------------------------------------------
    } else if(!strncmp(cbuf, "INS ", 4)) {
      fprintf(fpo, "%s\n", &cbuf[4]);


    //----------------------------------------------------------------
    } else {
      // skip all other commands ..
      printf(" skip |%s|\n",cbuf);
      continue;
    }

    ++iRnr;

  }

  TX_Print("G-Code: %d records written.",iRnr);

  fclose(fpi);
  fclose(fpo);
  if(fpLog) fclose(fpLog);



  //================================================================
  printf("===================================== \n");
#ifdef _MSC_VER
  sprintf(cbuf, "type \"%s\"",outFilnam);
#else
  sprintf(cbuf, "cat %s",outFilnam);
#endif

  system (cbuf);

  return 0;

}


/* ======================== EOF ====================== */
