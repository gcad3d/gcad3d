/* MemTab                            2003-05-20  franz.reiter@cadcam.co.at
../ut/ut_memTab.h
for Functions see ../ut/ut_memTab.c


*//*!
\file ../ut/ut_memTab.h
\ingroup grp_ut_obgrp_ut_obj
*/

/// \code
/// Typ_MemTab - template for the typedef:
/// data    startpos of memspc; typ = template-parameter # 1
/// rMax    max. nr of records; size of data is (rMax * rSiz) bytes
/// rNr     nr of used records in data
/// rSiz    recordsize in bytes
/// typ     type of data-records; for info only.
/// incSiz  if Memspc is too small: add UTI_round_b2i(incSiz * rSiz)
///         UINT_8_MAX = cannot realloc (fixed space)
/// use     application-specific
/// spcTyp  0=malloc-type=can-reallocate,must-free;
///         1=fixed-CANNOT-reallocate;must-free;
///         2=fixed-CANNOT-reallocate;must-NOT-free;
///         3=stack,CANNOT-reallocate,must-NOT-free;
/// \endcode
#define def_MemTab(vTyp1)\
  typedef struct {vTyp1 *data; int rMax, rNr, rSiz;\
          unsigned char typ, incSiz, use, spcTyp;}
// size = 20

/// template for the typedef:
#define typedef_MemTab(vTyp1)\
  def_MemTab(vTyp1) MemTab_##vTyp1

/// template for the typename:
#define MemTab(vTyp1) MemTab_##vTyp1
// #define MemTab(vTyp1) (MemTab_##vTyp1) = { ((void *)0), -1, -1}

// #define _MEMTAB_NUL {NULL, 0, 0, 0, ' ',' ',' ',' '}
#define _MEMTAB_NUL {NULL, 0, 0, 0}

/// default-typedef (mit void *data)
def_MemTab(void) MemTab;



//================================================================
// prototypes:
int MemTab_ini (void *memTab, int rSiz, int typ, int incSiz);
int MemTab_add (void *memTab, long *spcOff, void* objDat, int recNr, int mode);
// inline:
void MEMTAB_tmpSpc_get (MemTab *memTab, int rTot);
void MEMTAB_tmpSpc_free (MemTab*);
int MEMTAB_IND (MemTab*);
int MEMTAB_RMAX (MemTab*);           // get max. nr of records
int MEMTAB_RSIZ (MemTab*);
void *MEMTAB__ (MemTab *memTab, int ind);
void *MEMTAB_DAT (MemTab*);
void *MEMTAB_POS (MemTab*);
int MEMTAB_IND_POS (MemTab*, void*);


//================================================================
// inline functions:


/// \code
/// MEMTAB_tmpSpc_get   get memspace for rTot records
/// memspace exists only until active function returns.
/// memspace MUST BE FREED at end of active function (MEMTAB_tmpSpc_free)
/// Example:
///  MemTab(int) mti1;                        // memspc for int's
///  MEMTAB_tmpSpc_get (&mti1, 1000);         // get space for 1000 int's
///  ia = MEMTAB_DAT(&mti1);                  // get data-block-address
///  for(i1=0;i1<1000;++i1) ia[i1] = i1;      // store data
///  MEMTAB_IND(&mtd1) = 1000;                // set data 0-999 used
///  ...
///  MEMTAB_tmpSpc_free (&mti1);              // free data
/// \endcode
#ifdef _MSC_VER
#define MEMTAB_tmpSpc_get(mtb1,rTot)\
 (mtb1)->data=NULL;\
 (mtb1)->rSiz=sizeof(*(mtb1)->data);\
 (mtb1)->rNr=(rTot)*(mtb1)->rSiz;\
 if((mtb1)->rNr < SPC_MAX_STK) (mtb1)->data = _alloca ((mtb1)->rNr + 64);\
 if(!(mtb1)->data) {\
    (mtb1)->data = malloc ((mtb1)->rNr);\
    (mtb1)->spcTyp = 0;\
    (mtb1)->incSiz = (mtb1)->rNr / 2;\
  } else {\
    (mtb1)->spcTyp = 3;\
    (mtb1)->incSiz = UINT_8_MAX; }\
  (mtb1)->rMax = rTot;\
  (mtb1)->rNr = 0;
#else
#define MEMTAB_tmpSpc_get(mtb1,rTot)\
 (mtb1)->data=NULL;\
 (mtb1)->rSiz=sizeof(*(mtb1)->data);\
 (mtb1)->rNr=(rTot)*(mtb1)->rSiz;\
 if((mtb1)->rNr < SPC_MAX_STK) (mtb1)->data = alloca ((mtb1)->rNr);\
 if(!(mtb1)->data) {\
    (mtb1)->data = malloc ((mtb1)->rNr);\
    (mtb1)->spcTyp = 0;\
    (mtb1)->incSiz = (mtb1)->rNr / 2;\
  } else {\
    (mtb1)->spcTyp = 3;\
    (mtb1)->incSiz = UINT_8_MAX; }\
  (mtb1)->rMax = rTot;\
  (mtb1)->rNr = 0;
#endif

/// MEMTAB_tmpSpc_free     free memSpace from MEMTAB_tmpSpc_get
#define MEMTAB_tmpSpc_free(mtb1)\
  if((mtb1)->spcTyp < 2) free ((mtb1)->data);



// MEMTAB__            get pointer to data-record from index
// MemTab(ObjDB)  mtPar = _MEMTAB_NUL;  long dli;
// dli = (MEMTAB__(&mtPar,ii))->dlInd;  // get a part of ObjDB-data index ii
#define MEMTAB__(mtb,irec)\
 (&((mtb)->data[irec]))

// MEMTAB_IND          get or set index (next free = nr of used)      INLINE
// Example: see MEMTAB_tmpSpc_get__
#define MEMTAB_IND(mtb)\
 ((mtb)->rNr)

// MEMTAB_RESET_IND    reset index (nr of used = 0)                   INLINE
#define MEMTAB_RESET_IND(mtb)\
 ((mtb)->rNr=0)

// MEMTAB_RMAX         get max. nr of records                         INLINE
#define MEMTAB_RMAX(mtb)\
 ((mtb)->rMax)

// MEMTAB_RSIZ         get record size                                INLINE
#define MEMTAB_RSIZ(mtb)\
 ((mtb)->rSiz)

// MEMTAB_RFREE        get nr of free records                         INLINE
#define MEMTAB_RFREE(mtb)\
 ((mtb)->rMax - (mtb)->rSiz)

// MEMTAB_DAT          get data-record complete                       INLINE
#define MEMTAB_DAT(mtb)\
 ((mtb)->data)


// MEMTAB_POS          get position of next data-record               INLINE
// DOES NOT WORK IF data=NULL: use MemTab_check before.
// Example:
//  // check for space for 4 points 
//  MemTab_check (pmt, &l1, 4);
//  // get pointer to data
//  pa = MEMTAB_POS (pmt);
//  // save point-data
//  MemTab_sav (pmt,  &l1, points, 4);
//  // pa now is adress of points
#define MEMTAB_POS(mtb)\
 (&((mtb)->data[(mtb)->rNr]))


// MEMTAB_IND_POS      get record-index from position                 INLINE
#define MEMTAB_IND_POS(mtb,pos)\
 (((char*)pos - (char*)(mtb)->data) / (mtb)->rSiz)


// MEMTAB_IS_EMPTY      test if MemTab.data == NULL
#define MEMTAB_IS_EMPTY(mtb)\
  (mtb)->data == NULL


// EOF
