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

-----------------------------------------------------
TODO:



-----------------------------------------------------
Modifications:
2015-11-03 new ex Gis_u1.c. RF


*/
/*!
\file  ../ut/ut_log.c
\brief write log-messages into file, display file
\code
=====================================================
List_functions_start:

LOG_A__         write
LOG_A_pt2       report errPos
LOG_A_pt3       report errPos
LOG_A_write     write, fopen if not yet open; set AP_stat.logStat

LOG_A_set_fnam  (re)define logfilename
LOG_A_init      set logfilename
LOG_A_disp      display logfile
LOG_A_exit      close logfile, reset AP_stat.logStat

List_functions_end:
=================================================================

\endcode *//*----------------------------------------


Set the logfilename with LOG_A_set_fnam
Delete the logfile.

Example usage:

#include "../ut/ut_log.h"                  // ERR_*

  sprintf(fn, "%sLog_xy.txt", AP_get_tmp_dir());
  LOG_A_set_fnam (fn);          // fopen

  if(AP_stat.logStat)
  LOG_A__ (ERR_ERR, " err xyz - retCod=%d",irc);
    // Messagetypes for LOG_A__: see ../xa/xa_msg.h ERR_INF - ERR_CON
 
  LOG_A_exit ((int)errNr);      // close

  // display logfile manually: Dump/view_logfile or with func.
  LOG_A_disp ();




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
#include "../xa/xa_msg.h"              // ERR_*
#include "../xa/ap_stat.h"             // AP_STAT



// externals:
// xa.c:
extern AP_STAT   AP_stat;         // sysStat,errStat..

// local:
static FILE      *LOG_A_fp=NULL;        // file Logfile
static char      LOG_A_fnam[128];  // filename Logfile

static char      *LOG_A_txt[]={"INF ","WNG ","ERR ","****","  - "};


  int LOG_A__ (int msgTyp, char* txt, ...);

//================================================================
// Externals
// ex ../xa/xa_main.c
  char*  AP_get_tmp_dir   ();
  char*  OS_date1  ();



//================================================================
  int LOG_A_set_fnam (char* fnam, int iopen) {
//================================================================
// LOG_A_set_fnam   (re)define logfilename
// Input:
//   fnam     filename logfile
//   iopen    0 = do not (yet) open file;   1 = open file;
// Output:
//   retCode: 0 = OK;   -1 open-error;

  int  irc;

  // printf("LOG_A_set_fnam %s\n", fnam);

  if(LOG_A_fp) {
    fclose (LOG_A_fp);
    LOG_A_fp = NULL;
  }

  if(strlen(fnam) >= 128) {
    TX_Error ("LOG_A_set_fnam - filename too long ..");
    return -1;
  }
  strcpy(LOG_A_fnam, fnam);

  // if not yet open then open Logfile
  // open log-file
  if(iopen) irc = LOG_A__ (ERR_INF, "=========== logfile %s ",fnam);
  else      irc = 0;
  return irc;

}


//================================================================
  int LOG_A_write (char* s1) {
//================================================================
 

  // printf("LOG_A_write |%s|\n",s1);

  if(!LOG_A_fp) {
    if ((LOG_A_fp = fopen (LOG_A_fnam, "w")) == NULL) {
      TX_Print("***** CANNOT OPEN LOG-FILE |%s|",LOG_A_fnam);
      return -1;

    } else {
      AP_stat.logStat = 1; // ON
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
//   msgTyp     ERR_INF = 0        // see ../xa/xa_msg.h
//              ERR_WNG = 1
//              ERR_ERR = 2
//              ERR_CON   4        // continuation line
//   txt        must be format (eg "%s" or "%s %d")
//   ...        parameters appropriate format <txt>
//
// see also gis_msg__

 
  va_list va;
  char    s1[1024];


  strcpy(s1, LOG_A_txt[msgTyp]);

  va_start(va,txt);
  vsprintf(&s1[4], txt, va);
  va_end(va);

    // printf(" LOG_A__ |%s|\n",s1);

  return LOG_A_write (s1);

}


//================================================================
  int LOG_A_pt2 (Point2 *pte) {
//================================================================
// report errPos
  
  char   s1[80];


  sprintf(s1, "pos =  %lf %lf",pte->x, pte->y);

  // return LOG_A_write (s1);
  return LOG_A__ (ERR_CON, s1);

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
/// \code
/// disp logfile
/// close, display, open-append
/// \endcode

  int  istat=0;


  if(LOG_A_fp) {
    fclose (LOG_A_fp);
    istat = 1;
  }


  // OS_browse_htm (LOG_A_fnam);
  APP_edit (LOG_A_fnam, 1);

  if(istat) {
    if ((LOG_A_fp = fopen (LOG_A_fnam, "a")) == NULL) {
      TX_Print("***** CANNOT OPEN LOG-FILE");
      return -1;
    }
  }


  return 0;

}


//================================================================
  int LOG_A_init (char *appNam) {
//================================================================
// set automatic logfilename and open logfile

  char   s1[256];


  // open log-file
  sprintf (s1, "%s%s.log",AP_get_tmp_dir(),appNam);

  // set logfilename and open
  LOG_A_set_fnam (s1, 1);

  return 0;

}


//================================================================
  int LOG_A_exit (int errNr) {
//================================================================
/// if (errNr >= 0) then nr of errors is reported with TX_Print

  // close logfile
  if(LOG_A_fp) {
    fprintf (LOG_A_fp, "ENR %d errors\n",errNr);
    fprintf (LOG_A_fp, "INF =========== Logend %s\n",OS_date1());
    fclose (LOG_A_fp);
    LOG_A_fp = NULL;
    AP_stat.logStat = 0; // OFF

    if(errNr > 0) TX_Print ("**** Logfile reports %d errors",errNr);
  }

  return 0;

}


//================================================================
  int LOG_A_ck () {
//================================================================
// LOG_A_ck             check if logfile (yet) open; 1=yes, 0=no

  // close logfile
  if(LOG_A_fp) return 1;

  return 0;

}






// EOF
