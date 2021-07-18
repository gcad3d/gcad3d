/* CAD (InteractiveEditor)                  2001-08-10        RF

used with deactivated extern in ../xa/xa_cad_ui.c

#include "../xa/xa_cad_ui.h"           // IE_FUNC_IS_ACTIVE

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


typedef struct   { Point pos;          // P: thisPoint; L,C,Curv: selectionPoint
                   Vector vx, vz;      // Vector: thisVector, Angle: vx,vz
                   double d1;          // Angle: angle.
                   char auxInf[16];    // word 3 of .info; eg "DD0"
                   long dli;           // dli of preview-obj
                   int  typ,
                        dlTyp;         // 0=DB-obj, 1=temp-obj
                 } inpAuxDat;


//----------------------------------------------------------------
MemObj UI_cad  (MemObj *mo, void **data);

  int IE_cad_init0 ();
//   int IE_cad_exit0 ();
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



//----------------------------------------------------------------
#define IE_Func_Models     10
#define IE_Func_Transform  11    
#define IE_Func_Modify     12
#define IE_Func_Interact   13
#define IE_Func_CADEnv     14





//----------------------------------------------------------------
// Variables declared extern - intern for ../xa/xa_cad_ui.c:

/// active MenueGroup;  1=not-active; 0=PT, 1=LN .. ANALYZ=14 ..
extern int       IE_FuncTyp;





//----------------------------------------------------------------
#define INTPTYANZ     16

#define INPRECANZ      8         // Anzahl Eingabefelder

#define IE_MEN_NR 15       // total nr of menuGroups


// test if CAD-function is active;  1=yes, 0=no
#define IE_FUNC_IS_ACTIVE ((IE_FuncTyp >= 0) ? 1 : 0)


// IE_GET_INP_DLI       get dli for active inputField
// dli = IE_GET_INP_DLI (IE_inpInd);
#define IE_GET_INP_DLI(iind)  iind + 1
// returns (iind + 1)
// see also GR_TMP_I0 IE_get_inp_dli
// // #define IE_GET_INP_DLI(iind)  -iind - 2
// // dli f. Feld 0: -2, Feld 1: -3 ....



//============== EOF ============================

// EOF
