/* complex-obj ObjGX
../ut/ut_ox_base.h
Functions: ../ut/ut_ox_base.c
*/




  int OGX_ox_ato1 (ObjGX *oxo, int *ausTyp, double *ausTab);
// void OGX_SET_OBJ (void*, int, int, int, void*);
  long OGX_siz__ (ObjGX *oxi, int isolate);
  int OGX_siz_ox (long *oSiz, void *obj1, int typ, int iNr, int isolate);
  ObjGX* OGX_oxm_copy_ox (Memspc *mSpo, ObjGX *oTab, int iNr, int isolate);
  ObjGX* OGX_oxm_copy_obj (Memspc *mSpo,
                      void *obji, int typ, int iNr, int isolate);
  long OGX_is_dbo (ObjGX *oxi);



//----------------------------------------------------------------
/// OGX_SET_OBJ                create complexObject (ObjGX) from binObj (struct)
/// \code
/// replaces: ox.typ   = oTyp;
///           ox.form  = oForm;           // type of oDat
///           ox.siz   = oSiz;            // size of oDat
///           ox->data = (void*)oDat;
/// no serialize, delocate; see OGX_ox_copy..
/// \endcode
void OGX_SET_OBJ (ObjGX *ox, int oTyp, int oForm, int oSiz, void *oDat);
#define OGX_SET_OBJ(ox,oTyp,oForm,oSiz,oDat){\
  (ox)->typ = (oTyp);\
  (ox)->form = (oForm);\
  (ox)->siz = (oSiz);\
  (ox)->data = (oDat);\
  (ox)->dir = 0;}


/// OGX_GET_OBJ             get type and data of binary-obj
/// replaces: oTyp = ox.form;
///           oDat = ox->data;
///           oNr  = ox.siz;            // number of records in *data;
void OGX_GET_OBJ (int *oTyp, void **oDat, int *oNr, ObjGX *ox);
#define OGX_GET_OBJ(oTyp,oDat,oNr,ox){\
  *(oTyp) = (ox)->form;\
  *(oDat) = (ox)->data;\
  *(oNr)  = (ox)->siz;}


/// OGX_SET_OBJ_aux            create complexObject (ObjGX) with aux.data
/// \code
/// aux is used with binary-meshes for the type of faces;
/// replaces: ox.typ   = oTyp;
///           ox.form  = oForm;           // type of oDat
///           ox.siz   = oSiz;            // number of records in *data;
///           ox->data = (void*)oDat;     // data; structure = oForm
///           ox->aux  = (int)aux;        // eg GL_TRIANGLE_FAN
/// \endcode
void OGX_SET_OBJ_aux (ObjGX *ox, int typ, int form, int siz, void *dat, int aux);
#define OGX_SET_OBJ_aux(ox,oTyp,oForm,oSiz,oDat,iaux){\
  (ox)->typ = (oTyp);\
  (ox)->form = (oForm);\
  (ox)->siz = (oSiz);\
  (ox)->data = (oDat);\
  (ox)->dir = 0;\
  (ox)->aux = (iaux);}




/// \brief OGX_SET_INDEX  create complexObject (ObjGX) from DB-Obj (dbTyp,dbInd)
/// \code
/// replaces: ox.form  = Typ_Index;
///           ox.typ   = dbTyp;
///           ox->data = (void*)dbInd;
/// \endcode
void OGX_SET_INDEX (ObjGX *ox, int dbTyp, long dbInd);
#define OGX_SET_INDEX(ox,dbTyp,dbInd){\
  (ox)->typ = (dbTyp);\
  (ox)->form = Typ_Index;\
  (ox)->siz = 1;\
  (ox)->data = (void*)(dbInd);}


/// \brief OGX_GET_INDEX            get index out of complexObject
/// \code
/// replaces: (int)ind = (long)oxi->data;   (if oxi->form == Typ_Index)
/// \endcode
/// see also LONG_PTR
void OGX_GET_INDEX (int *typ, long *dbi, ObjGX *ox);
#define OGX_GET_INDEX(dbTyp,dbInd,ox){\
  *(dbTyp) = (ox)->typ;\
  *(dbInd) = (long)(ox)->data;}



/// \brief OGX_SET_Float8  create complexObject (ObjGX) from (typ, float8)
/// \code
/// replaces: ox.form  = f8typ;
///           ox.typ   = Typ_Float8;
///           ox->data = *float8;          // pointer to static double 
/// typ can be Typ_Val|Typ_Par1|Typ_Angle
/// \endcode
void OGX_SET_Float8 (ObjGX *ox, int f8typ, double *float8);
#define OGX_SET_Float8(ox,f8typ,float8){\
  (ox)->typ = (f8typ);\
  (ox)->form = Typ_Float8;\
  (ox)->siz = 1;\
  (ox)->data = (void*)(float8);}



/// \brief OGX_SET_INT  create complexObject (ObjGX) from (typ, int4)
/// \code
/// replaces: ox.form  = Typ_Int4;
///           ox.typ   = dbTyp;
///           ox->data = (void*)int4;
/// \endcode
void OGX_SET_INT (ObjGX *ox, int typ, int ival);
#define OGX_SET_INT(ox,dbTyp,ii4){\
  (ox)->typ = (dbTyp);\
  (ox)->form = Typ_Int4;\
  (ox)->siz = 1;\
  (ox)->data = (void*)(long)(ii4);}



/// \brief OGX_GET_INT              get index out of complexObject
/// \brief OGX_GET_LONG             get index out of complexObject
/// \code
/// replaces: (int)ind = (long)oxi->data;   (if oxi->form == Typ_Index)
/// \endcode
#define OGX_GET_INT(ox) (long)(ox)->data
#define OGX_GET_LONG(ox) (long)(ox)->data



/// \brief OGX_SET_COLOR       create complexObject (ObjGX) from (typ, int4)
/// \code
/// replaces: ox.form  = Typ_Int4;
///           ox.typ   = Typ_Color;
///           ox->data = (void*)int4;
/// \endcode
void OGX_SET_COLOR (ObjGX *ox, ColRGB *col);
#define OGX_SET_COLOR(ox,col){\
  (ox)->typ = Typ_Color;\
  (ox)->form = Typ_Int4;\
  (ox)->siz = 1;\
  (ox)->data = (void*)(*((long*)(col)));}


/// \brief OGX_GET_COLOR      get pointer to Typ_Color
// #define OGX_GET_COLOR(ox)\
  // (ColRGB)(ox)->data


/// OGX_is_dbo              check if struct is a DB-obj
/// returns  0 (oxi is NOT dbo) else the DB-index
#define OGX_is_dbo(oxi) ((oxi)->form == Typ_Index ? (long)(oxi)->data : 0L)


// INCLUDE_FULL defined only in ../ut/ut_ox_base.c
#define _OGX_NUL { NULL, (short)0, (short)0, (int)0 }
#ifndef INCLUDE_FULL
extern ObjGX OGX_NUL;
#else
ObjGX OX_NUL = _OGX_NUL;
#endif


// OGX_DAT                  get data-block of ObjGX
#define OGX_DAT(ogx)\
 ((ogx)->data)


// EOF
