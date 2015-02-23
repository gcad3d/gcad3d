/* MemTab                            2003-05-20  franz.reiter@cadcam.co.at

*//*!
\file ../ut/ut_memTab.h
\ingroup grp_ut_obgrp_ut_obj
*/
// see also ../ut/ut_memTab.c

/// \code
/// Typ_MemTab - template for the typedef:
/// data    startpos of memspc; typ = template-parameter # 1
/// rMax    size of data is (rMax * rSiz) bytes
/// rNr     nr of used records in data
/// rSiz    recordsize in bytes
/// typ     type of data-records; for info only.
/// incSiz  if Memspc is too small: add UTI_round_b2i(incSiz * rSiz)
/// use     application-specific
/// \endcode
#define def_MemTab(vTyp1)\
  typedef struct {vTyp1 *data; int rMax, rNr, rSiz;\
          unsigned char typ, incSiz, use, u4;}

/// template for the typedef:
#define typedef_MemTab(vTyp1)\
  def_MemTab(vTyp1) MemTab_##vTyp1

/// template for the typename:
#define MemTab(vTyp1) MemTab_##vTyp1
// #define MemTab(vTyp1) (MemTab_##vTyp1) = { ((void *)0), -1, -1}

// #define MemTab_empty {NULL, 0, 0, 0, ' ',' ',' ',' '}
#define MemTab_empty {NULL, 0, 0, 0}

/// default-typedef (mit void *data)
def_MemTab(void) MemTab;



//================================================================
// prototypes:
int MEMTAB_IND (MemTab*);
int MEMTAB_MAX (MemTab*);
void *MEMTAB__ (MemTab*, int);
void *MEMTAB_DAT (MemTab*);
void *MEMTAB_POS (MemTab*);
int MEMTAB_IND_POS (MemTab*, void*);


//================================================================
// inline functions:

// get data-record
#define MEMTAB__(mtb,irec)\
 (&((mtb)->data[irec]))

// MEMTAB_IND          get index (pointer to next free = nr of used)  INLINE
#define MEMTAB_IND(mtb)\
 ((mtb)->rNr)

// MEMTAB_MAX          get size                                      INLINE
#define MEMTAB_MAX(mtb)\
 ((mtb)->rSiz)

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

// EOF
