/* ../inf/Objects-Create.c

TODO: Update - see ../inf/GR_RECORDS.c

================================================================== */
void INF_GEOM_OBJ_CR(){                   /*! \code

See also INF_GR_RECORDS    (grafic recordtypes)


INF_Source-record
    * is a definition-line in modelcode, ascii-text.
    * is stored / reloaded with modelfile
    * Program execution creates a DB-record and a DL-record from a Source-
      record.


INF_Fixed-Database-record
    * is a binary object in the DB, defined by type (eg Typ_LN and index (dbi))
    * necessary for later use of the obj (create point, use as startpoint of
      ..)
    * is created from Source-records (definition-codes), program-calls
    * Database-records are not visible and are not filed with the model.


INF_Dynamic-Database-record
    * is a binary object in the dynamic DB-space, defined by type (eg Typ_LN
      and a negative index) the negative index is automatically created
    * does NOT have a Source-record
    * is used for test-output, intermediate results / objects ..


INF_DisplayList-record
    * is a binary object in the DL
    * DisplayList-records are necessary for visible objects
    * DisplayList-records are not filed with the model.


INF_Temporary-DisplayList-record   (eg 
    * have type Typ_dynSym; 
    * cannot be hilited.


INF_ApplicationObjects
    * used for plugin-specific objects
    * use DisplayList-records with type Typ_APPOBJ can be seen / created /
      modified in plugins ../dox/ApplicationObjects.dox


INF_ApplicationData
    * used for plugin-specific data can be created / modified in plugins ../
      dox/ApplicationData.dox


================================================================== \endcode */}
void INF_Source-record (){        /*! code
=====================================================================

Create:
// add a single codeline in memory, process code.
// Creates a source-record, a Database-record and a DisplayList-record
ED_srcLn_add ("L1=P(0 0 0) P100 0 0)", 0);
UTO_sav_ost // create DB-object & save it in Model (from bin. obj)

-----------------------------------------------------------
// create permanent objects from sourceObj (ascii-text):
// add source-records into a buffer,
// add buffer to model and process new codes.
// Creates source-records, Database-records and DisplayList-records
// clear global buffer UTF_FilBuf1
UTF_clear1 ();
// Init objNrs
AP_obj_2_txt (NULL, 0L, NULL, 0L);
// add 2objcts to UTF_FilBuf1
// using buffer mem_cbuf1 (../xa/xa_mem.h)
sprintf(mem_cbuf1,"P1=0 0 0");
UTF_add1_line (mem_cbuf1);
sprintf(mem_cbuf1,"P2=100 200 0");
UTF_add1_line (mem_cbuf1);
// output UTF_FilBuf1 -> CAD
UTF_insert1 (-1L);
// update display (work new created objects)
APED_update__ (-1L);
// update browser-window (display newly created objects)
Brw_Mdl_upd ();

-----------------------------------------------------------
// create permanent objects from binary objects (complexObj):
Point p2={1.0, 1.0, 0.0};
ObjGX o1;
// clear UTF_FilBuf1
UTF_clear1 ();
// Init objNrs
AP_obj_2_txt (NULL, 0L, NULL, 0L);
// create a complexObj
OGX_SET_OBJ (&o1, Typ_PT, Typ_PT, 1, &p2);
// create sourceObj from complexObj in mem_cbuf1; add to UTF_FilBuf1
// ind -1: get next free ..
i1 = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &o1, -1L);
printf(" %d |%s|\n",i1,mem_cbuf1);
// add objNam to last sourceObj in UTF_FilBuf1
UTF_add1_last_add (" #Mittelpkt oben");
UTF_dump1__ (); // dump UTF_FilBuf1
// add a commentline to UTF_FilBuf1
sprintf(mem_cbuf1,"# this is a test ..");
UTF_add1_line (mem_cbuf1);
// output UTF_FilBuf1 -> CAD
UTF_insert1 (-1L);
// update display (work new created objects)
APED_update__ (-1L);

-----------------------------------------------------------
// create source-record:
UTF_add_line (src); // add code to model


=====================================================================
Modify:
=====================================================================
APED_src_chg // modify & add to undo-list
// update DB & display
APED_update__ (lNr);
// see also UTF_chg_line // modify line in memory
WC_Work__ (lNr, src); // process code

-----------------------------------------------------------
AP_stru_2_txt // create sourceObj from complexObj
UTF_add1_last_add // add name to last obj in UTF_FilBuf1
UTO_sav_ost // create DB-object & save it in Model.


=====================================================================
Delete:
=====================================================================

UTF_del_line1 (lNr); // delete source-line; get lNr with APED_search_defLn
// or from DisplayList-record

-----------------------------------------------------------
OBSOLETE
// // example create with undo for the plugin-output:
// // UNDO for a block of code
// UNDO_app__ (0); // init undo (get act.lNr)
// AP_obj_2_txt ..
// UTF_add1_line ..
// UTF_insert1 .. // add code to mainBuffer
// Brw_Mdl_upd (); // update browser-window
// UNDO_app__ (1); // create undo-record; activate undo-button
// return 0; // exit plugin

-----------------------------------------------------------
UNDO_.. if stored in do/undo-list
// UNDO for single lines of code
// add sourceObj's of sourcelines 4 and 8 to do/undo-list
UNDO_grp_add (4L, 0);
UNDO_grp_add (8L, 1);
// delete the active undo-grp & update display
UNDO_grp_del ();
// see also Del_tab__
// alle temporaer geloeschten Zeilen entfernen (und Undo-Liste entleeren)
// (temporaer geloeschten Zeilen beginnen mit "_")
AP_APT_clean ();
// Das gesamte Textfile loeschen (Funktion File/New)
UI_menCB (NULL, "new");


================================================================== \endcode */}
void INF_Fixed-Database-record (){        /*! code
===============================================================================
Create:
dbi = DB_StoreLine (123L, &ln1); // store as "L123"
DB_QueryNxtFree DB_GetDynInd DB_isFree_PT // find free dbi
DB_StorePoint, DB_store_obj
Modify:
DB_GetObjDat


================================================================== \endcode */}
void INF_Dynamic-Database-record (){        /*! code
===============================================================================
DB_dump_stat ();
Create:
dbi = DB_StorePoint (-1L, &pt1); // returns the (negative) dbi
GR_Cre.. create dynamic-DB-obj, DL-record and display obj
----------------------------------------------------------------
long dynPti, dbi;
Point pt1 = {12.0, 0.4, 0.0};
dynPti = DB_dyn__ (0, Typ_PT, 0L); // save state of dyn-points
dbi = DB_StorePoint (-1L, &pt1); // store dynamic obj in DB.
printf(" dyn.ind=%ld\n",dbi); // returns DB-index
// use point ..
pt1 = DB_GetPoint (dbi);
UT3D_stru_dump(Typ_PT, DB_get_PT(dbi), "dyn pt");
// remove (all folowing) dynamic point(s)
DB_dyn__ (2, Typ_PT, dynPti); // reset state of dyn-points


================================================================== \endcode */}
void INF_DisplayList-record (){        /*! code
===============================================================================
see INF_DisplayList

Alt+Shift+d dump DL DL_DumpObjTab

Create:
// create new or overwrite DispList-record; returns its index
// if obj does not have DB-obj: use dbi = -1L
dli = DL_StoreObj (Typ_.., dbi, iAtt); //
GL_DrawSymB (&dli, ..
APT_disp_.. create dynam.DB-record, DL-record, display obj.
GR_Disp_.. create dynam.DB-record, DL-record, display obj.
#include "../gr/ut_UI.h" // SYM_..
Point p1={10,0,0};
long dli;
GR_Disp_pt (&pt1, SYM_STAR_S, 2); // creates temp.obj red circle
DL_Redraw (); // update display
dli = GL_temp_iNxt (); // get DL-index of circle
..
GL_Delete (dli); // delete obj and all following
GR_tDyn_obj tempDisp obj from typ+struct
GR_tDyn_dbo tempDisp obj from typ+dbInd
GR_Disp_ox temp. display of ObjGX-structs
GR_Disp_pt,-vc,-vc2,-ln,-ac,-bsp,-pln
GR_tDyn_txiA disp integer-chars
GR_tDyn_box__disp 3D-boundingBox from 2 points
UI_disp_Pos temporary display position (red circle)
UI_disp_vec1
UI_disp_tra
GL_DrawAngA draw angle with arrowhead
GR_Disp_pln GL_DrawSymVX display plane / axisSystem
UI_disp_pln_oid hilite plane from objID (disp RX RY RZ)
UI_disp_joint UI_disp_activ display objName as text
Modify:
DL_SetInd (dli); // modify, do not create new DL-Record with foll. DL_StoreObj
DL_dlRec__dli get DL-record
Delete:
GL_Del0 (dli); // delete single object
GL_Delete (dli); // delete this dl-record and all following records


================================================================== \endcode */}
void INF_Temporary-DisplayList-record (){        /*! code
===============================================================================
// Use next free dispListindex:
dli = -1;
// or get the next free index for direct use:
dli = GL_temp_iNxt ();
// or use fixed temporary index 2 (-2 to -(DL_base_font1 - 1)):
long dli = -2; // DispListIndex temp.
GR_tDyn_pcv (&dli, 1, pNr, pa);
..
GL_temp_Delete (-2L); // delete
GR_Draw.. (&dli, .. // display obj. Do not create DB-record, DL-record.
Delete:
GL_temp_Delete (dli);


================================================================== \endcode */}
void INF_ApplicationObjects (){        /*! code
===============================================================================


Darstellung von Applikationsspezifischen Objekten in Zusatzanwendungen.
Die Funktion zur Darstellung des Objekts liegt in der Zusatzanwendung.
Die Funktion hat Zugriff auf OpenGL.
Die Objekte sind somit nur mit der Zusatzanwendung darstellbar.

ApplicationObjects werden nicht in der Datenbasis gespeichert,
  sie müssen als ApplicationData gespeichert werden oder normalen
  Standardobjekten hinterlegt werden.

ApplicationObjects in der DispList: (DL_Att .)
  alle:                .typ   Typ_APPOBJ.
  Objekttypspezifisch: .iatt; zB. subTyp; 0=point, 1=line, 2=image ..
  Objektspezifisch:    .ind;  zB. ID, ObjNr.




Funktionen:
AP_UserAppObjNamTab         // provide names for application-objects
  // wie diese Objects in der Multi-SelektionsListe angezeigt werden ..


Beispiele:
APP_GIS1 gis_DrawPoint gis_DrawEL gis_EL_disp gis_sel_CB



Example:

  long dli = -1L;      // init; i1=dispList-record not yet exists.
  int subTyp = 1;      // dla.iatt
  int objId = 1;       // dla.ind
  DL_SetObj (&dli, Typ_APPOBJ, objId, subTyp);
  GR_tDyn_pcv (&dli, Typ_Att_hili1, pNr, pa);    
  or
  GL_DrawSymB (&dli, ATT_COL_GREEN, SYM_STAR_S, &pt);

  // hide obj 
  DL_unvis_set (dli, 1); // 0=vis, 1=unvis

  // delete all appObj's with all objId's with with <subTyp>
  GL_Del_om (Typ_APPOBJ, -1L, subTyp);  





Examples:

#define APPTYP_XY 0
#define APPTYP_XZ 1

  static APP_OBJ_NAM appObjNamTab[]={
    {"XY-Obj",  APPTYP_XY},   // 0
    {"XZ-Obj",  APPTYP_XZ},   // 1
    {"",-1}
  };


  long   id, dli;


  AP_UserAppObjNamTab (appObjNamTab); // provide names for application-objects


  // create App-Obj:
  id = 123;
  // test if App-Obj with this id already exists in the DispList
  dli = DL_find_APPOBJ (id, APPTYP_PT);
  if(dli >= 0) DL_SetInd (dli);       // modify (do not create new DL-Record)
  dli = DL_StoreObj (Typ_APPOBJ, id, APPTYP_XY); // create/modify DL-Record
  GL_DrawSymB (&id, 2, SYM_TRI_S, &p1);          // red triangle



// Test for App-Obj:
  DL_dlRec__dli (&dla, dli);
  if(dla.typ == Typ_APPOBJ) ..

// Test subTyp of App-Obj:
  if(dla.iatt == APPTYP_XY) ..

// get ID of App-Obj:
  id = dla.ind;
  if(id == 123) ..

// find App-Obj:
  id = 123;
  dli = DL_find_APPOBJ (id, APPTYP_XY);
  // delete App-Obj
  if(dli >= 0) GL_Del0 (dli);

// delete all App-Obj's of typ APPTYP_XY:
  GL_Del_om (Typ_APPOBJ, -1L, APPTYP_XY);

  GL_Regen1 ();           // reset DL




================================================================== \endcode */}
void INF_ApplicationData (){        /*! code
===============================================================================


Daten für Zusatzanwendungen (ApplicationData) können im Model gespeichert werden.
ApplicationData werden beim Laden eines Models wieder zur Verfügung gestellt.
Die Applikation muss vor dem Verlassen der Anwendung die Daten schreiben.
  Die Daten sind als ascii-text zu schreiben.


ApplicationData is stored with the model (as ascii-text).

Store ApplicationData:
  open file with appdat_open__(&fp, "w")
  write data (as ascii-text !)

Restore ApplicationData:
  open file with appdat_open__(&fp, "r")
  read data (as ascii-text !)

See demo-application ../APP/Demo_appDat1.c



    Datei <tempDir>/<appNam>.appdat schreiben.
  Der Applikationsname <appNam> ist frei wählbar.


------------------------------------------------------------------------
TODO:
- a direct way to store binary data ..
if value == "BINDATA" + <sizeInBytes>\n  then next line is binary data.



------------------------------------------------------------------------
Formatbeschreibung ApplicationData:
Die Zeilen SECTION und SECTIONEND befinden sich nur in der Modelldatei.


SECTION APPDAT <appNam>
.. (data)
SECTIONEND





affected functions:
Mod_kill__     löschen
Mod_load__     laden
Mod_sav__      speichern


================================================================== \endcode */}
// eof
