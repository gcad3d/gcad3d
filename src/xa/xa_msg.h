/* Messages.
needs
#include <stdarg.h>                    // va_list

#include "../xa/xa_msg.h"              // MSG_*

Messagetext: ../../doc/msg/msg_const_de.txt




-----------------------------------------------------
TODO:

- make errocode for "parameter-error"


- different languages for errorcodes MSG_ERR_tab in ../xa/xa_msg.c


// errorcodes               severity                               iErr
  ERR_internal = -99,       // 3  internal error, exit right nowa   -99

  ERR_TODO_E,               // 2  error, exit right now             -98

  ERR_TODO_I,               // 1  info only - TODO                  -97
  ERR_func_not_impl,        // 1  "function not implemented"
  ERR_subModel_undefined,   // 1  subModel_undefined
  ERR_db_obj_undefined,     // 1  db-obj_undefined
  ERR_file_open,            // 1  file_open error

  ERR_unsupported           // 0


// severity:
//   3  error-internal; exit right now, stop and unload plugin
//   2  error-unrecoverable; stop/break active operation;
//   1  error-continue next level;
//   0  error-continue normal; info only;


// - ERR_get_severity from errorCode
#define ERR_severity_3     ERR_internal
#define ERR_severity_2     ERR_TODO_E
#define ERR_severity_1     ERR_file_open

  if(iErr <= ERR_internal)        iSev = 3;   // MSG_ERR_typ_BRK
  else if(iErr <= ERR_TODO_E)     iSev = 2;   // MSG_ERR_typ_ERR
  else if(iErr <= ERR_file_open)  iSev = 1;   // MSG_ERR_typ_WNG
  else                            iSev = 0;   // MSG_ERR_typ_INF

// MSG_ERR_typ_xx == severity; MSG_ERR_typ_BRK
// text zu severity MSG_ERR_txt
 MSG_ERR_typ_ERR


es sollte verknüpft sein:
    errorCode severity textMessage
eg   -99       1       internal error

.......................................
Example:
    int irc = ERR_TODO_E;
    MSG_ERR__ (irc, "%d %d",i1,i2);
    return irc;
  }
  if(irc < ERR_severity_2)  return -1;



-----------------------------------------------------

*/




//================================================================
// ERROR-MESSAGES 
//================================================================


// severity from errorCode;
//   3  error-internal; exit right now, stop and unload plugin
//   2  error-unrecoverable; stop/break active operation;
//   1  error-continue next level;
//   0  error-continue normal; info only;
// eg: if(iErr <= ERR_severity_2) printf(" severity is 2 or 3\n");
#define ERR_severity_3     ERR_internal
#define ERR_severity_2     ERR_TODO_E
#define ERR_severity_1     ERR_file_open


// messagetype for MSG_ERR_out; see MSG_ERR_txt in ../xa/xa_msg.c
#define MSG_ERR_typ_INF     0
#define MSG_ERR_typ_WNG     1
#define MSG_ERR_typ_ERR     2
#define MSG_ERR_typ_BRK     3

int MSG_ERR_out (int msgTyp, const char *fnc, int ikey, char *txt, ...);


/// \code
/// MSG_ERR__             errormessage (key, additional_text)
/// Input:
///   iErr      errorcode; see enum ERR_codes below
/// Output:     TX_Error "<MSG_ERR_txt> <actFuncNam>(): <MSG_STD_tab[ikey]> ..."
///   retCod    ??
/// Example:  return MSG_ERR__ (func_not_impl, "/ cvTyp %d", cvTyp);
///   how to add new STD-message:
///    - add key in enum above;
///    - add text for key in MSG_ERR_tab in ../xa/xa_msg.c
///
///  return MSG_ERR__ (ERR_internal, "memspc not free");
/// \endcode
int MSG_ERR__ (int iErr, ...);
#define MSG_ERR__(iErr,...)\
 MSG_ERR_out (MSG_ERR_typ_ERR,__func__,iErr,__VA_ARGS__)


// keys for error-Messages MSG_ERR__;
// text for messages in MSG_ERR_tab in ../xa/xa_msg.c
enum ERR_codes {
  ERR_internal = -99,       // 3  internal error, exit right now    -99
  ERR_EOM,                  // 2  out of memory, exit right now     -98
  ERR_TODO_E,               // 2  error, exit right now             -97
  ERR_TODO_I,               // 1  info only - TODO                  -96
  ERR_func_not_impl,        // 1  function not implemented
  ERR_subModel_undefined,   // 1  subModel_undefined
  ERR_db_obj_undefined,     // 1  db-obj_undefined
  ERR_file_open,            // 1  file_open error
  ERR_TEST,                 // 1  testExit
  ERR_obsolete,             // 0
  ERR_unsupported           // 0
};

// MSG_ERR_sev                   get severity (1|2|3) from errorcode (< 0)
// iSev = 0 = INF     continue
// iSev = 1 = WNG     Error, continue
// iSev = 2 = ERROR   exit right now 
// iSev = 3 = BREAK   exit right now 
// see MSG_ERR_txt
#define MSG_ERR_sev(iErr)\
 ((iErr<=ERR_severity_1)?((iErr<=ERR_severity_2)?((iErr<=ERR_severity_3)?3:2):1):0)





//================================================================
// SYSTEM-MESSAGES 
//================================================================
// keys for constant-messages; MSG_const__ ()
// messageText:  ../../doc/msg/msg_const_en.txt
// modify messages: cd ../../doc/msg/  vi msg_const_*.txt
// MSG_SIZE must be last.
enum {
  MSG_ok,     MSG_cancel, MSG_save,    MSG_insert,      MSG_delete,     // 0-
  MSG_modify, MSG_exit,   MSG_help,    MSG_length,      MSG_activate,   // 5-
  MSG_move,   MSG_edit,   MSG_rename,  MSG_CreSubModel, MSG_show,       // 10-
  MSG_hide,   MSG_shade,  MSG_inspect, MSG_movPoints,   MSG_undo,       // 15-
  MSG_filter, MSG_texture,MSG_open,    MSG_GrpClear,    MSG_GrpMove,    // 20-
  MSG_GrpHide,MSG_GrpDelA,MSG_GrpAdd1, MSG_GrpRem1,     MSG_GrpRemTyp,  // 25-
  MSG_GrpRemOTs,MSG_upd,  MSG_all,     MSG_no,          MSG_Error,      // 30-
  MSG_cpyMdl, MSG_getPar, MSG_getChd,  MSG_modNam,      MSG_cvEd,       // 35-
  MSG_GrpInv, MSG_SIZE
};


  char* APP_MSG_get_0 (char *key);
  char* APP_MSG_get_1 (char *key, char *fmt, ...);
  char* MSG_get_0 (char *key);
  char* MSG_const__ (int iMsg);
  char* MSG_read (char *sbuf, int bufSiz, char *key);
  char* MSG_fread (char *sbuf, int bufSiz, char *key, FILE *fpIn);
  char* MSG_get_str (char *keyStr);
  int   MSG_get__ (char *msg, int msgSiz, char *key, FILE *fpIn,
                   char *fmt, va_list *va);


/*
// char **MSG_ERR_txt; ee ../xa/xa_msg.c
extern char *MSG_ERR_txt[];

// char **MSG_STD_tab; see ../xa/xa_msg.c
extern char *MSG_STD_tab[];
*/


// MS-VS-2010:
#ifndef __func__ 
#define __func__ __FUNCTION__
#endif



// EOF
