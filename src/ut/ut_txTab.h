

# define TAB_C_SIZ 1024
# define TAB_I_SIZ 32
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
typedef struct {long tabSiz, indSiz; char *tab;
                int iNr, stat; long *ind;}                   TxtTab;


#define _UTXTAB_NUL {0,0, NULL, 0,0, NULL}

/// \code
/// setup of a new StringList.
/// creates: TxtTab xTab = {0,0, NULL, 0,0, NULL};
/// \endcode
#define UtxTab_NEW(xTab)\
   TxtTab xTab = _UTXTAB_NUL

//_____________________________________
// PROTOTYPES:
  char* UtxTab__        (int iNr, TxtTab *tab);
  char* UtxTab_pos_free (TxtTab *xTab);

  int UtxTab_add (TxtTab *tab, char *newtxt);

// EOF
