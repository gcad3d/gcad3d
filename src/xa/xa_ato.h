// ../xa/xa_ato.c
// prerequis: ../ut/ut_mem.h


void ATO_tmpSpc_get__ (ObjAto *ato, int sizTab);
int ATO_getSpc_siz    (int sizTab);
void ATO_getSpc_tmp1  (ObjAto *ato, int sizTab);
void ATO_getSpc_tmp__ (ObjAto *ato, int sizTab);
void ATO_getSpc_tmp_s (ObjAto *ato, char ptx);


#define _OBJATO_NUL {0,0,0, NULL, NULL, NULL, NULL }



/// \code
/// ATO_tmpSpc_get__                get memspace for atomicObjects (nr-bytes)
/// Input: siz   size of type/value-table
/// Example: ObjAto ato; ATO_tmpSpc_get__ (&ato, 6); ATO_tmpSpc_free (&ato);
/// \endcode
// #ifdef _MSC_VER
// #define ATO_tmpSpc_get__(ato1,rNr)\
// printf("*** ERR MEM_alloc_tmp |%s| %d\n",__func__,rNr);
// #else

#ifdef _MSC_VER
#define ATO_tmpSpc_get__(ato1,bNr)\
 if((bNr) < SPC_MAX_STK) (ato1)->typ=(int*)_alloca(bNr + 64);\
 ATO_tmpSpc_get1(ato1,bNr)
#else
#define ATO_tmpSpc_get__(ato1,bNr)\
 if((bNr) < SPC_MAX_STK) (ato1)->typ=(int*)alloca(bNr);\
 ATO_tmpSpc_get1(ato1,bNr)
#endif


/// \code
/// ATO_tmpSpc_get_s                get memspace for atomicObjects for string
/// get memspace on stack or heap 
/// Input:  ptx   point to sourcecode
/// Output: (ato1)->siz  - nr of records necessary
///         (ato1)->nr   - nr of bytes necessary
/// \endcode
#ifdef _MSC_VER
#define ATO_tmpSpc_get_s(ato1,ptx)\
 (ato1)->siz = SRCU_tsMax (ptx);\
 (ato1)->nr = ATO_getSpc_siz ((ato1)->siz);\
 if((ato1)->nr < SPC_MAX_STK) (ato1)->typ=(int*)_alloca((ato1)->nr + 64);\
 ATO_tmpSpc_get1(ato1,(ato1)->nr)
#else
#define ATO_tmpSpc_get_s(ato1,ptx)\
 (ato1)->siz = SRCU_tsMax (ptx);\
 (ato1)->nr = ATO_getSpc_siz ((ato1)->siz);\
 if((ato1)->nr < SPC_MAX_STK) (ato1)->typ=(int*)alloca((ato1)->nr);\
 ATO_tmpSpc_get1(ato1,(ato1)->nr)
#endif


// ATO_tmpSpc_free        free memSpace from MemTab_ini_temp
// see MEM_MUST_FREE
#define ATO_tmpSpc_free(ato1)\
  if((ato1)->spcTyp == MEMTYP_ALLOC__) free ((ato1)->typ);


/// \code
/// ATO_getSpc_tmp__                get memspace for atomicObjects;
/// memspace exists only until active function returns.
/// Input: siz   size of type/value-table
/// Example: ObjAto ato; ATO_getSpc_tmp__ (&ato, 6);
/// \endcode
#define ATO_getSpc_tmp__(ato1,siz)\
 (ato1)->typ=(int*)MEM_alloc_tmp(ATO_getSpc_siz(siz));\
 ATO_getSpc_tmp1(ato1,siz)


/// ATO_getSpc_tmp_ck        test if got memspace
#define ATO_getSpc_tmp_ck(ato1)\
  (ato1)->typ


/// ATO_clear__             delete all records
void ATO_clear__ (ObjAto *ato1);
#define ATO_clear__(ato1) (ato1)->nr=0


#define _ATO_NUL {0, 0, 0, NULL, NULL, NULL, NULL}

// EOF
