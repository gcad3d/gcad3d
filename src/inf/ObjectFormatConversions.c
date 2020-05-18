/* ../inf/ObjectFormatConversions.c
See also:
../../doc/gcad_doxygen/ObjectFormatConversions.dox


================================================================== */
void INF_OBJ_CONV__(){                   /*! \code
INF_OBJ_CONV__      ObjectFormatConversions


INF_objectTypes     eg Typ_PT
INF_objectIDs       eg "P123"
INF_objectName      eg "#height 2 floor"
INF_sourceLines     editor functions
INF_sourceObjects   text;expressions            eg "P12" "DZ" "L(P10 D(0 0 1))"
INF_atomicObjects   ausTyp/ausTab-array;           (*int, *double)
INF_binaryObjects   type & dedicated structure; eg ((int)Typ_PT, (Point)pt)
INF_DB-Objects      type & DB-index;               (int, long)
INF_DL-Objects      DL-index;                      (long)
INF_complexObject   struct ObjGX;                  (ObjGX)

Files:
NC_apt.c     CommandInterpreter - decode-functions          (APT_decode_)
ut_gtypes.c  typChar, typ, name(typ,index), ..
ut_ox_base.c complex-object-manipulationes (copy, relocate) (OGX_)
ut_obj.c     object-manipulationes (transform, convert ..)  (UTO_)
xa_obj_txt.c convert object-struct -> source-text           (AP_obj_)
xa_src.c     check, modify, convert source-objects (text)   (SRC_)
see also ResolveObjects


================================================================== \endcode */}
void INF_objectTypes (){        /*! code


 objectTyp and objectID
  objTyp (int) or (char)  Typ_PT   'P'         _typ_
    "P12"       P is the objectType (Typ_PT)

AP_typ_typChar           make typ from typChar ("P" -> Typ_PT)
AP_typChar_typ           make typChar from typ (Typ_PT -> 'P')
AP_typDB_typ             give basictyp from typ (eg Typ_SUR from Typ_SURRU)
APED_dbo_oid             give typ and index from objectID eg "P12"

AP_typ_srcExpr           decode expression
AP_typ_FncNam            get type of function
AP_src_typ__             get objTypTxt from typ
DB_Typ_Char              give typ(int) from text "PT" --> Typ_PT;
AP_cmp_typ               check for identical types
UTO_ck_oTyp              check object-typ (struct or object)
UTO_ck_typTyp            check if typ=curve (Typ_lFig) or surface (Typ_SUR)
UTO_ck_curvLimTyp        check if CurveLimits are Points or parameters
UTO_ck_curvForm          check if objTyp is ObjGX or other typ
UTO_ck_surfTyp           returns surfSubTyp

AP_ck_ftyp               returns fileTyp as int; 0=native, 1-9=Import, 10-19=Mockup
AP_i2ftyp                returns fileTyp as text


================================================================== \endcode */}
void INF_sourceObjects (){        /*! code
INF_sourceObjects   text;expressions            eg "P12" "DZ" "L(P10 D(0 0 1))"

L(P10 D(0 0 1))    is a dynamic object  osrc





================================================================== \endcode */}
void INF_objectIDs (){        /*! code


 objectID = objectTyp and DB-index
  objID  (char*)                   "P123"      _oid_
    "P12"       P is the objectType (Typ_PT)


APED_oid_dbo__         get objectID from typ and DB-Index (visible object types)
APED_oid_dbo_all       get objectID from typ and DB-index (all types)
SRC_src_dbo            create sourceCode of requested type from Database-object
APED_oid_vc            get oid for Vector; (DX or DIX or D#)
AP_obj_add_obj         add objname to string
AP_obj_add_pt          add Point* as "P(xyz)"
AP_obj_add_vc          add struct Vector* to string " D(x y z)"
AP_obj_add_val         add double > text ( xa_obj_txt.c )
AP_cre_defHdr          create new (unused) objHeader
AP_src_typ__           get typText from typ (eg "PT" for Typ_PT)
AP_src_typMod          get modifier-text from (modifier)typ
APT_decode_print       create sourceObj from atomicObjs (for PRInt-cmd)


================================================================== \endcode */}
void INF_objectName (){        /*! code


is      eg "#height 2 floor"
APED_lNr_objNam get sourcelineNr from ObjName
APED_onam_search search objName between ps and pe
APED_onam_cut cut/copy objName


================================================================== \endcode */}
void INF_sourceLines (){        /*! code

source (asciiText with objectIDs and functions ..) _src_

functions - result is sourceLine
UTF_GetLinNr            get a copy of a sourceObj-line
UTF_GetPosLnr           get a pointer to a sourceObj-line
AP_obj_2_txt            change obj to text and save it with UTF_add1_line
AP_stru_2_txt           change object into text (definition-line)
APT_decode_func SQRT SIN COS TAN ASIN ACOS ATAN ABS FIX RND
APED_ck_defLn           check if Line cBuf is a Definitionline
SRC_src_ato             sourceObj (text) from atomicObjs
APED_txo_srcLn__        analyze sourceline; get typ,form,level


================================================================== \endcode */}
void INF_atomicObjects (){        /*! code


atomicObjects (ausTyp/ausTab, array) _ato_:
(list of objTyp and list of objValues)
ATO_ato_srcLn__ get atomicObjects from sourceLine; full evaluated.
ATO_ato_txo__ get atomic-objects from source-objects
ATO_ato_eval__ evaluate atomic-objects (compute); reduce records.
ATO_ato_eval_geom evaluate geometrical functions
APT_decode__ sourceObj -> atomicObj (ausTyp/ausTab-array)
ATO_sort1
ATO_getSpc__
ATO_getSpc1
ATO_srcTxt get atomicObj from Typ_Txt (after AP_typ_srcExpr)
ATO_swap swap 2 records
APT_decode_ausdr decode sourceObj -> atomicObj
APT_decode_ausdr1 operators constantObjects constantValues
SRC_ato_SIZ, SRC_ato_typ, SRC_ato_tab = global memspc


================================================================== \endcode */}
void INF_binaryObjects (){        /*! code

binObj (dedicated structure) pt|ln..:

obj = binary-obj ((int)typ, (void*)data)

(typ - (int) eg Typ_PT for (Point), Typ_VC for (Vector)
(structure; the type of the structure must be known)

--------- get binaryObject from DB-Obj
DB_GetObjDat     get dataStruct from DB-Obj Resolv Refs. Give pointer.

--------- get binaryObject from complexObj
UTO_obj_getp     get dataStruct from ObjGX Resolv Refs. Give pointer.
UTO_obj_get      get dataStruct from ObjGX Resolv Refs. Give a copy.

--------- get binaryObject from sourceObj
APT_obj_expr     Create struct from ModelCode (text).
APT_decode_objTx Create struct from ModelCode (text).
UT3D_pt_txt      get point from text
UT3D_vc_txt      get vector from text
AP_vec_txt       give vector-struc from vector-text (only defvecs)
APT_obj_expr
APT_obj_ato
APT_decode_pt APT_decode_ln APT_decode_ci ..
APT_decode_angd__ APT_decode_goAxis APT_decode_goRadius (../ci/NC_apt.c)

UTO_ck_surfTyp returns surfSubTyp (see also SUR_ck_typ)
UTO_siz_stru get size of structure
UTO_copy_stru copy struct (no resolve - not recursive)


================================================================== \endcode */}
void INF_DB-Objects (){        /*! code


DB-Obj: _dbo_    objectType and dataBaseIndex
DB-struct: _dbs_ data for VDPLC, ox for SAB (this format is stored in the DB)


UTO_ck_dbsTyp       check object-typ (struct (V,D,P,L,C) or object (S,A,B))
APED_dbo_oid        get DB-Obj (typ and index) from sourceObj eg "P12"
DBO_dbo_src__       get DB-Obj (typ and index) from sourceObj eg "L(S1 MOD(1))"
UTO_dbs_ox          DB-struct (data for VDPLC, ox for SAB) from ox
DBO_sel__           change selection into DB-object
DBO_dump__          dump DB-object -> debug-window


DB-objects from atomicObj's:
ATO_ato_eval__      create dynam DB-obj from atomic-obj
see also APT_decode_pt*


================================================================== \endcode */}
void INF_DL-Objects (){        /*! code


DL_DumpObjTab
DL_dlRec__dli get DL-record
DL_dli__dbo
DL_find_APPOBJ


================================================================== \endcode */}
void INF_complexObject (){        /*! code


complexObj (ObjGX) _obj_
(ObjGX = objType + structure ..)
../ut/ut_ox_base.h
OGX_SET_OBJ create complexObject (ObjGX) from binObj (struct)
OGX_SET_INDEX create complexObject (ObjGX) from DB-Obj (dbTyp,dbInd)
OGX_GET_INDEX get typ,dbi out of ObjGX
OGX_siz_obj return size of complete ObjGX-Tree;
UTO_obj_save copy complexObj -> Memspc
UTO_copy_obj copy complexObj -> memory
UTO_isol__ duplicate/isolate object (resolve Links)
UTO_reloc_obj relocate obj (oGX-tree)


================================================================== \endcode */}
// eof
