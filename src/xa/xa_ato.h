

int ATO_getSpc_tmpSiz (int sizTab);
void ATO_getSpc_tmp1 (ObjAto *ato, int sizTab);
void ATO_getSpc_tmp__ (ObjAto *ato, int sizTab);

/// ATO_getSpc_tmp__                get memspace for atomicObjects;
/// memspace exists only until active function returns.
/// Input: siz   size of type/value-table
/// Example: ObjAto ato; ATO_getSpc_tmp__ (&ato, 6);
#define ATO_getSpc_tmp__(ato1,siz)\
 (ato1)->typ=(int*)UME_alloc_tmp(ATO_getSpc_tmpSiz(siz));\
 ATO_getSpc_tmp1(ato1,siz)



/// ATO_clear__             delete all records
void ATO_clear__ (ObjAto *ato1);
#define ATO_clear__(ato1) (ato1)->nr=0


// EOF
