/*        utilityprogs for postprocs

see ut/ut_TX.c
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

#include "pp_ut.h"


extern FILE *fpLog;


///==========================================================
  void TX_Print (char* txt, ...) {
///==========================================================
/// \code
/// formatted write into messagewindow.
/// Example:
///   TX_Print (".. distance is %lf", d1);
/// \endcode


  va_list va;
  char    TX_buf1[1024];


  // if(TX_mode == OFF) {
    // return;
  // }

  va_start(va,txt);

  vsnprintf(TX_buf1,sizeof(TX_buf1),txt,va);

  va_end(va);

  if(strlen(TX_buf1) > 100) {
    TX_buf1[100] ='\0';
  }

  TX_Write (TX_buf1);

}


///==========================================================
  void TX_Error (char* txt, ...) {
///==========================================================
/// formatted errortext into messagewindow.
/// Example:
///   TX_Print (".. distance must not exceed %lf", d1);


  char   TX_buf1[1024];


  va_list va;

  va_start(va,txt);


  strcpy(TX_buf1, "*** Fehler: ");
                // o123456789012
  vsnprintf(&TX_buf1[12],sizeof(TX_buf1)-12,txt,va);
  va_end(va);

  TX_Write (TX_buf1);


}


///==========================================================
  void TX_Write (char* txt) {
///==========================================================
///  direkt rausschreiben.


  printf("%s\n",txt);

  if(fpLog) fprintf(fpLog, "%s\n", txt);

}


//===========================================================
  char* UTX_CleanCR (char* string) {
//===========================================================
/// \code
/// UTX_CleanCR              Delete Blanks, CR's and LF's at end of string
/// returns positon of stringterminator \0
/// 
/// see also UTX_del_follBl UTX_CleanSC
/// \endcode


  int  ilen;
  char *tpos;

  ilen = strlen (string);

  tpos = &string[ilen];

  if(ilen < 1) goto L_exit;

  --tpos;


  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    if(tpos <= string) goto L_exit;
    --tpos;
  }

  ++tpos;


  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return tpos;
}


// EOF
