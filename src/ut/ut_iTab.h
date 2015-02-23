

/// tab     space for integers (int*)
/// tabSiz  size of tab (integers)
/// iNr     next free position (= nr of integers in tab)
/// stat    0=fixed size(cannot reallocate); 1=automatic reallocting; 2=file
typedef struct {long tabSiz, iNr; int stat, *tab;}                     I4Tab;



#define I4Tab_NEW(iTab)\
   I4Tab iTab = {0,0,0,NULL}


// EOF
