/* ../inf/Userinteractions.c
../../doc/gcad_doxygen/Objects-Resolve.dox


================================================================== */
void INF_GEOM_OBJ_RES(){                   /*! \code



INF_Get_DispListObject       DL_Att
INF_Get_DataBaseObject       (BinaryObject or ComplexObject)
INF_Get_SourceObj            _src_ (obj as asciiText)

see also INF_OBJ_CONV__  ObjectFormatConversions


================================================================== \endcode */}
void INF_Get_DataBaseObject (){        /*! code


DataBaseObjects are defined by the objTyp typ and the dataBaseIndex dbi.
objID is a string of the objTyp and the dataBaseIndex, eg "P12"
Functions handling objTyp dataBaseIndex objID: see ObjectFormatConversions
BinaryObject         dedicated structure; Point|Line|Circ ...
ComplexObject        ObjGX - structure see ComplexObject
ut_DB.c              all DB - functions
// get objID as string from dispListIndex (eg "P1")
DL_oid__dli (oID, dli);
printf(" objID=|%s|\n",oID);
// get db-typ and db-ind from dispList-ind
DBO_sel__ (&typ, &dbi, dl_ind);
DL_find_sel // get typ,dbi of first selected (hilited) obj
Functions to get objects from the DB:
DB_GetObjGX get complexObj from DB-Obj Keep Refs. Give ObjGX.
UTO_obj_getp get dataStruct from ObjGX Resolv Refs. Give pointer.
UTO_obj_get get dataStruct from ObjGX Resolv Refs. Give a copy.
UTO__dbo get dataStruct from DB-Obj Resolv Refs. Give pointer.
int typ;
long dli, dbi;
DL_Att dla;
void *oo;
// get DB-complex-obj
ox1 = DB_GetObjGX (typ, dbi);
// ptp1 = DB_get_PT (21L);
pt = DB_GetPoint (dbi);
// lnp1 = DB_get_LN (20L);
// cip1 = DB_get_CI (5L); // Typ_CI
// pln1 = DB_get_PLN (20L);
// ox1p = DB_GetCurv (20L);
// get surface
// ox1p = DB_GetSur (20L, 0);
// find all depending sourceObjs
APED_find_dep__
DB_Stat(); // print Statistics
DB_dump__ (); // beware ..
DB_QueryDef // check if obj defined ..
DB_QueryLastInd // letzten definierten Index holen
DB_QueryNxtInd // nächsten freien Index retournieren
// Testausgabe einer Objektstruktur:
UT3D_stru_dump (int typ, void *data, char *txt, ...);
UT3D_stru_dump (Typ_PT, &pTab[i1], "Point[%d]=",i1);


================================================================== \endcode */}
void INF_Get_SourceObj (){        /*! code


// get sourcelineNr from dispListNr
DL_Get_lNr_dli (&lNr, dli); // get lNr from DL-ind
printf(" lNr=%d\n",lNr);
// get sourceline & sourcelineNr from ObjID
int lnNr, lnLen;
strcpy(oID, "R4");
irc = APED_search_defLn (&lPos, &lnNr, &lnLen, oID, -1, 0);
printf(" irc=%d lNr=%d ll=%d\n",irc,lnNr,lnLen);
// get a copy of a sourceline from sourcelineNr
UTF_GetLinNr
// get startpos and lineLength of sourceline from sourcelineNr
UTF_GetPosLnr


================================================================== \endcode */}
void INF_Get_DispListObject (){        /*! code


DispList is (DL_Att*) GR_ObjTab;
// get DL-record from dispListIndex dli
DL_dlRec__dli (&dla, dli);
typ = dla.typ; // DB-typ
dbi = dla.ind; // DB-ind
dla.modInd; // -1=active Model, 0-n=Submodel
// get dispListIndex (dli) of last created obj
GL_GetActInd ();
// get dispListIndex from sourceLineNr
DL_Get_dli_lNr (&dli, &lNr);
// get dispListIndex of (last) DB-object from DB-obj (typ/dbi)
dli = DL_dli__dbo (typ, dbi);
// dump DispList
DL_DumpObjTab ();


================================================================== \endcode */}
// eof

