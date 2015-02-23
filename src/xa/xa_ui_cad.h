/* CAD (InteractiveEditor)                  2001-08-10        RF

*//*!
\file ../xa/xa_ui_cad.h
\ingroup grp_ap
*/


/// \brief menuRecordHeader
/// \code
/// ftxt  functionDescription (eg "PT cartes")
/// ftyp  functionType        (eg curves - "POL" or "BSP")
/// \endcode
typedef struct {char* ftxt; char* ftyp;}             IE_rec_txt;


/// \brief menuRecordHeader
/// \code
/// ind  functionNr
/// typ  requested typ 
/// info Infotext; [ = optional, < = group
/// \endcode
typedef struct {int ind; int typ; char* info;}       IE_rec_stru;



MemObj UI_cad  (MemObj *mo, void **data);

  int IE_cad_init0 ();
  int IE_cad_exit0 ();
  int IE_cad_OK    ();

char* IE_get_inp_  (int ind);
int   IE_sel_CB__  (int typ, long ind, char *buf);
int   IE_sel_CB_1  (int typ, long ind, char *buf);
int   IE_activate  ();

//int IE_ed1__     (MemObj *mo, void **data);
//int IE_ed1_CB    (MemObj *mo, void **data);
//int IE_ed1_mod   (MemObj *mo, void **data);

  int IE_cad_test1 ();
  int IE_inp_set_menu_col (int mode, int ind, char *txt);

  int IE_inp_CB__  (MemObj *mo, void **data);
int   IE_cad_InpIn_CR ();
int   IE_cad_InpIn_CB (int ind, int key);
  int IE_cad_ClearInputs ();

int   IE_inp_chg (int mode);

  int IE_del_start (void *parent, void *data);

  int IE_decode_Ln (int *typTab, char txtTab[][256], int tabSiz, char *cbuf);

  int IE_auxFun1 (char **obuf, char **tbuf, IE_rec_txt *ibuf);


//============== EOF ============================
