/* Messages.
needs
#include <stdarg.h>                    // va_list

#include "../xa/xa_msg.h"              // MSG_*

Messagetext: ../../doc/msg/msg_const_de.txt
*/




int MSG_STD_ERR (int ikey, ...);

// keys for standard-Messages MSG_STD_ERR;
// text for messages in MSG_STD_tab in ../xa/xa_msg.c
enum {
  ERR_func_not_impl,        ///< "function not implemented"
  ERR_subModel_undefined,   ///< subModel_undefined,
  ERR_db_obj_undefined,     ///< db-obj_undefined,
  ERR_file_open,            ///< file_open error
  ERR_internal,             ///< internal error
  ERR_unused
};


// messagetype for MSG_STD__; see MSG_STD_code in ../xa/xa_msg.c
#define MSG_typ_INF     0
#define MSG_typ_WNG     1
#define MSG_typ_ERR     2

int MSG_STD__ (int msgTyp, const char *fnc, int ikey, char *txt, ...);





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
// char **MSG_STD_code; ee ../xa/xa_msg.c
extern char *MSG_STD_code[];

// char **MSG_STD_tab; see ../xa/xa_msg.c
extern char *MSG_STD_tab[];
*/


// MS-VS-2010:
#ifndef __func__ 
#define __func__ __FUNCTION__
#endif

/// \code
/// MSG_STD_ERR             errormessage (key, additional_text)
/// Input:
///   ikey    see enum below
/// Output:   TX_Error "<MSG_STD_code> <actFuncNam>(): <MSG_STD_tab[ikey]> ..."
/// Example:  return MSG_STD_ERR (func_not_impl, "/ cvTyp %d", cvTyp);
///   how to add new STD-message:
///    - add key in enum below;
///    - add text for key in ../xa/xa_msg.c MSG_STD_tab
/// \endcode

#define MSG_STD_ERR(ikey,...)\
 MSG_STD__ (MSG_typ_ERR,__func__,ikey,__VA_ARGS__)


// EOF
