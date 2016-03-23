/***************************************************************************
 *   Copyright (C) 2015 by Franz Reiter   *
 *   franz.reiter@cadcam.co.at   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*

../ut/ut_log.c                          2015-10-08    Franz Reiter.
../ut/ut_log.h

-----------------------------------------------------
TODO:



-----------------------------------------------------
Modifications:
2015-11-03 new ex Gis_u1.c. RF


=====================================================
Liste_Funktionen_Beginn:

LOG_A__
LOG_A_pt2       report errPos
LOG_A_pt3       report errPos
LOG_A_write

LOG_A_set_fnam  (re)define logfilename
LOG_A_init
LOG_A_disp
LOG_A_exit

Liste_Funktionen_Ende:
=================================================================


Example usage:

#include "../ut/ut_log.h"                  // MSG_typ_*

  LOG_A__ (MSG_typ_ERR, " err xyz - retCod=%d",irc);



*/


#ifdef _MSC_VER
#include <windows.h>    // Sleep
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_log.h"                  // MSG_typ_*




static FILE      *LOG_A_fp=NULL;        // file Logfile
static char      LOG_A_fnam[128];  // filename Logfile

static char      *LOG_A_txt[]={"INF ","WNG ","ERR "};


//================================================================
// Externals
// ex ../xa/xa_main.c
  char*  OS_get_tmp_dir   ();
  char*  OS_date1  ();



//================================================================
  int LOG_A_set_fnam (char* fnam) {
//================================================================
// LOG_A_set_fnam   (re)define logfilename


  if(LOG_A_fp) {
    fclose (LOG_A_fp);
    LOG_A_fp = NULL;
  }

  strcpy(LOG_A_fnam, fnam);

  return 0;

}


//================================================================
  int LOG_A_write (char* s1) {
//================================================================
 

  if(!LOG_A_fp) {
    if ((LOG_A_fp = fopen (LOG_A_fnam, "w")) == NULL) {
      // TX_Print("***** CANNOT OPEN LOG-FILE");
      printf("%s\n",s1);
      return -1;

    } else {
      fprintf (LOG_A_fp, "INF Logstart %s\n",OS_date1());
    }
  }

  fprintf(LOG_A_fp, "%s\n",s1);

  return 0;

}


//================================================================
  int LOG_A__ (int msgTyp, char* txt, ...) {
//================================================================
// LOG                    write message into logfile
// Input:
//   msgTyp     MSG_typ_INF = 0
//              MSG_typ_WNG = 1
//              MSG_typ_ERR = 2
//
// see also gis_msg__

 
  va_list va;
  char    s1[1024];


  strcpy(s1, LOG_A_txt[msgTyp]);

  va_start(va,txt);
  vsprintf(&s1[4], txt, va);
  va_end(va);


  return LOG_A_write (s1);

}


//================================================================
  int LOG_A_pt2 (Point2 *pte) {
//================================================================
// report errPos
  
  char   s1[80];


  sprintf(s1, "PT2 %lf %lf",pte->x, pte->y);

  return LOG_A_write (s1);

}   
  


//================================================================
  int LOG_A_pt3 (Point *pte) {
//================================================================
// report errPos
 
  char   s1[80];


  sprintf(s1, "PT3 %lf %lf %lf",pte->x, pte->y, pte->z);

  return LOG_A_write (s1);

}


//================================================================
  int LOG_A_disp () {
//================================================================
// disp logfile
// close, display, open-append


  if(LOG_A_fp) fclose (LOG_A_fp);

  // APP_browse__ (LOG_A_fnam);
  APP_edit (LOG_A_fnam);

  if ((LOG_A_fp = fopen (LOG_A_fnam, "a")) == NULL) {
    TX_Print("***** CANNOT OPEN LOG-FILE");
  }


  return 0;

}


//================================================================
  int LOG_A_init () {
//================================================================

  // open log-file
  sprintf (LOG_A_fnam, "%sGIS1.log",OS_get_tmp_dir());

  return 0;

}


//================================================================
  int LOG_A_exit (int errNr) {
//================================================================
// if (errNr >= 0) then nr of errors is reported with TX_Print

  // close logfile
  if(LOG_A_fp) {
    fprintf (LOG_A_fp, "INF Logend %s\n",OS_date1());
    fclose (LOG_A_fp);
    LOG_A_fp = NULL;
    TX_Print ("  Logfile %s written",LOG_A_fnam);

    if(errNr >= 0) TX_Print ("**** Logfile reports %d errors",errNr);

  }

  return 0;

}


// EOF
