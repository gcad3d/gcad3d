//                                                        2010-08-27   RF
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
\file  ../ut/ut_err.c
\brief crash handler
\code
=====================================================
List_functions_start:

ERR_init
ERR_RESET
ERR_cb1

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


longjmp geht nur wenn die Funktion, in der setjmp gerufen wurde,
  noch nicht verlassen wurde !
Feststellen, ob der Stackpointer tiefer liegt, geht nicht richtig,
  da die handlerRoutine auch einiges am Stack ablegt.
  Bei Linux64 ist dieser Offset leider nicht immer gleich gross;
    geht daher nicht.

---------------------------------------------------
Linux-32:
  signal geht nur einmal; es funktioniert nur sigaction.

MS-Win - gcc:
  signal muss neu aktiviert werden; geht dann ..

Linux-64:
  wie Linux-32.


see also ../ut/tst_err1.c
see also raise sigsetjmp siglongjmp sigaction




---------------------------------------------------
usage:

int funcXY () {

  int    i1;

  i1 = ERR_SET1 ();
  if(i1) {
    printf("Fehler in protected_routine1\n");
    goto L_exit;
  }

  // dangerous code ..
  i1 = 10. / 0.;

  L_exit:
  ERR_RESET ();

}


*/

#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
#include <signal.h>
#include <setjmp.h>
#define ERR_SET1 setjmp(err_buf)
static jmp_buf       err_buf;
*/
#define extern          // damit wird "extern" im Includefile geloescht !
#include "../ut/ut_err.h"
#undef extern           // reset extern ..


// static struct sigaction err_sDfl;

// Protos:
 void ERR_cb1 (int i1);



//================================================================
  int ERR_init () {
//================================================================
// 


  // printf("ERR_init \n");

#ifdef _MSC_VER
  // geht mit MS/gcc ned ..
  signal (SIGSEGV, ERR_cb1);      // 11
  // // signal (SIGFPE,  ERR_cb1);      //  8


#else
  struct sigaction   ssa;

  ssa.sa_handler = ERR_cb1;
  sigemptyset (&ssa.sa_mask);
  // ssa.sa_flags   = SA_RESTART | SA_NOMASK | SA_NOCLDSTOP;
  ssa.sa_flags   = SA_ONESHOT | SA_NOMASK;

  sigaction (SIGSEGV, &ssa, NULL);      // 11
  // sigaction (SIGSEGV, &ssa, &err_sDfl);      // 11
  // sigaction (SIGFPE,  &ssa, NULL);      // 8
    // printf(" Signal %d\n",i1);
    // printf(" sizeof jmp_buf = %d\n",sizeof(jmp_buf));

#endif


  return 0;

}


//================================================================
  int ERR_RESET () {
//================================================================
 
  // printf("ERR_RESET \n");


#ifdef _MSC_VER
  // geht mit MS/gcc ned ..
  signal (SIGSEGV, SIG_DFL);

#else
  struct sigaction   ssa;

  ssa.sa_handler = SIG_DFL;
  sigemptyset (&ssa.sa_mask);
  ssa.sa_flags   = 0;

  sigaction (SIGSEGV, &ssa, NULL);      // 11
  // sigaction (SIGFPE,  &ssa, NULL);      // 8
#endif


  // // sigaction (SIGSEGV, &err_sDfl, NULL); 

  return 0;
}


//================================================================
 void ERR_cb1 (int i1) {
//================================================================
// Input:
//   i1   signalID


  printf("ERR_cb1 i1=%d\n",i1);


  ERR_RESET ();  // normally the default


  if(i1 == SIGSEGV) {        // 11
    printf("***** SegmentViolation *****\n");

  } else if(i1 == SIGFPE) {  // 8
    printf("***** FloatingPointException *****\n");

  }

  // jump to stored adress ..
  // if(act_sp <= err_sp) {
// #ifdef _MVC_VER
    // signal (i1, ERR_cb1);  // reassign
// #endif
    longjmp (err_buf, 1);

  // } else {
    // printf(" jumpBackData NOT VALID !\n");
    // exit(0);
  // }

}


// EOF
