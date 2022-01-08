/*
../ut/ut_txTab.h
../ut/ut_txTab.c
*/

# define TAB_C_SIZ 12288
# define TAB_I_SIZ 384
// this is 32 chars for each string; 36 chars total for a string ..


/// \brief  StringList (a class for stacking Textstrings)
/// \code
/// tab     space for strings (char*)
/// tabSiz  size of tab (chars)
/// ind     integerTable; charNr (Startpostion) of string in tab
/// indSiz  size of ind
/// iNr     next free position in ind (= nr of strings in tab
/// stat    0=fixed size(cannot reallocate); 1=automatic reallocting
/// \endcode
typedef struct {int tabSiz, indSiz; char *tab;
                int iNr, stat; int *ind;}                   TxtTab;


#define _UTXTAB_NUL {0,0, NULL, 0,0, NULL}

// setup of a new StringList.
// creates: TxtTab xTab = {0,0, NULL, 0,0, NULL};
#define UtxTab_NEW(xTab)\
   TxtTab xTab = _UTXTAB_NUL


// check if UtxTab is new or already has memspc; 1=new,empty; 0=has_memspc
#define UtxTab_IS_NEW(xTab) (((xTab)->tabSiz) ? 0 : 1)

// get free space in TxtTab.tab (not TxtTab.ind)
#define UtxTab_TABSPC(xTab) (xTab->tabSiz - xTab->ind[xTab->iNr])


//_____________________________________
// PROTOTYPES:
  char* UtxTab__        (int iNr, TxtTab *tab);
  char* UtxTab_pos_free (TxtTab *xTab);

  int UtxTab_add (TxtTab *tab, char *newtxt);

// EOF
