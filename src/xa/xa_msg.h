/* constant messages. MSG_const__ ()
needs
#include <stdarg.h>                    // va_list

#include "../xa/xa_msg.h"              // MSG_*

Messagetext: ../../doc/msg/msg_const_de.txt
*/

// MSG_SIZE must be last.
enum {
  MSG_ok,     MSG_cancel, MSG_save,    MSG_insert,      MSG_delete,     // 0-
  MSG_modify, MSG_exit,   MSG_help,    MSG_length,      MSG_activate,   // 5-
  MSG_move,   MSG_edit,   MSG_rename,  MSG_CreSubModel, MSG_show,       // 10-
  MSG_hide,   MSG_shade,  MSG_inspect, MSG_movPoints,   MSG_undo,       // 15-
  MSG_filter, MSG_texture,MSG_open,    MSG_GrpClear,    MSG_GrpHide,    // 20-
  MSG_GrpDelA,MSG_GrpAdd1,MSG_GrpRem1, MSG_GrpRemTyp,   MSG_GrpRemOTs,  // 25-
  MSG_upd,    MSG_all,    MSG_no,      MSG_Error,       MSG_cpyMdl,     // 30-
  MSG_getPar, MSG_getChd, MSG_modNam,                                   // 35-
  MSG_SIZE
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


#define MSG_typ_INF     0
#define MSG_typ_WNG     1
#define MSG_typ_ERR     2


/// keys for MSG_STD__ MSG_STD_ERR, add messages to MSG_STD_tab in ../xa/xa_msg.c
enum {
  func_not_impl,        ///< "function not implemented"
  subModel_undefined,   ///< subModel_undefined,
  file_open,            ///< file_open error
  unused
};


// char **MSG_STD_code;
extern char *MSG_STD_code[];
// char *MSG_STD_code[]={
  // "-",             // INF
  // "***",           // WNG
  // "*** ERROR:"     // ERR
// };


/// messages for MSG_STD__ MSG_STD_ERR - in file ../xa/xa_msg.c
// char **MSG_STD_tab;
extern char *MSG_STD_tab[];
// char *MSG_STD_tab[]={
  // "function not implemented",    ///< 0 func_not_impl
  // "uu"
// };


/// \code
/// MSG_STD_ERR             errormessage (key, additional_text)
/// Input:
///   ikey    see eg func_not_impl in ../xa/xa_msg.h
/// Output:   TX_Error "<MSG_STD_code> <actFuncNam>(): <MSG_STD_tab[ikey]> ..."
/// Example:  return MSG_STD_ERR (func_not_impl, "/ cvTyp %d", cvTyp);
/// \endcode

int MSG_STD_ERR (int ikey, ...);
int MSG_STD__ (int msgTyp, const char *fnc, int ikey, char *txt, ...);

// MS-VS-2010:
#ifndef __func__ 
#define __func__ __FUNCTION__
#endif

#define MSG_STD_ERR(ikey,...)\
 MSG_STD__ (MSG_typ_ERR,__func__,ikey,__VA_ARGS__)


// EOF
