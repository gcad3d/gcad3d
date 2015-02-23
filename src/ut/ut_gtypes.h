
//----------------------------------------------------------------


int APED_dbo_oid (int *defTyp, long *defInd, char* txtIn);
int APED_oid_dbo__ (char *buf, int typ, long ind);
// char* DB_Char_Typ (int typ);
char* AP_src_typ__ (int typ);
char* AP_src_typMod (int typ);
int    DB_Typ_Char      (char* auxBuf);
int DB_2DTyp_3DTyp (int typIn);
int AP_typ_typChar (char typChar);
char AP_typChar_typ (int typ);
int AP_typ_2_bastyp (int typ);
int AP_cmp_typ (int typ1, int typ2);
int UTO_ck_dbsTyp (int typ);
int UTO_ck_typTyp (int typ);
int UTO_ck_curvLimTyp (int typ);
int UTO_ck_curvForm (int typ);
int UTO_ck_surfTyp (ObjGX *oxi);

// EOF
