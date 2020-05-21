/* ../inf/GR_RECORDS.c

TODO:
  - Update - see ../inf/Objects-Create.c INF_GEOM_OBJ_CR

  - GR_perm_|GR_test_|GR_temp_
    - get DL-index DL_dli_perm|
    - DL-record speichern DL_set__  (was DL_StoreObj)
    - get GL-index GL_gli_perm|GL_gli_test|DL_temp_init
    - GL-record speichern GL_set_<typ>



--------------------------------------------------------------------
Source-record    - see INF_Source-record
    is a definition-line in modelcode, ascii-text.
    is stored / reloaded with modelfile
    Program execution creates a DB-record and a DL-record from a Source-record.
    Example : "P20=P(100 0 0)"  - defines a point at coord. 100,0,0

Database-record
    is a binary record in DB, one for euch visible object.
    is defined by type (eg Typ_LN) and index (dbi)
    stores the coordinates and all necessary values and parameters.
    Database-records are not filed with the model.
    Create DB-records: DB_store_obj()

DisplayList-record

    is a binary object in the DL, one for euch visible object.
    stores the visibility-attributes (hilited, dimmed, hidden, in-group, ..)
    DisplayList-records are not filed with the model.
    Create DL-records: DL_StoreObj()
    Overwrite DL-records already-in-use: DL_SetInd().



================================================================== */
void INF_GR_RECORDS (){        /*! \code


//================================================================
Permanent-Object                   GR_OTYP_PERM
//================================================================

    Has Source-record, has DB-record, has DL-record,
    create binary-obj from Source-record, store binary-obj in DB

  Create Permanent-objects:
    GR_perm_*() - display-functions to create DL-record; displist-index > 1
      displist-typ is the DB-typ,
      displist-dbi is the DB-index (positive)
      OpenGL-index starts at DL_base_mod; 

  Modify Permanent-objects:
    overwrite existing record with DL_SetInd() setting DL_perm_ind



//================================================================
Dynamic-Object
//================================================================

  Does NOT have a Source-record with explicite object-id;

  =========================================
  Permanent-Dynamic-objects    GR_OTYP_PERM
  =========================================
  - have a DB-record, have a DL-record; no Source-record
    no Source-record - is not filed in the model
    used for intermediate results, but objects later used
    can be selected, analyzed, have negative DB-index;
  Example: L20=P20 P(100,50,0); // the second point is a Permanent-Dynamic-object

  Create Permanent-Dynamic-objects:
    GR_perm_*() - display-obj-functions; 
      displist-typ is the DB-typ,
      displist-dbi is the DB-index (negative);
      OpenGL-index starts at DL_base_mod; 

  Modify Permanent-Dynamic-objects:
    overwrite existing record with DL_SetInd() setting DL_perm_ind


  ==========================================
  Temporary-Dynamic-objects     GR_OTYP_TDYN
  ==========================================
  - have a DL-record; no DB-record, no Source-record
    analyze gives type, DL-index,
    used for test-output (unlimited nr of objs)

  Create Temporary-Dynamic-objects:
    GR_tDyn_*() - display obj;
      displist-typ is Typ_dynSym,
      displist-dbi is the DL-index (for indentification when selected)
        DL_Att.irs,iatt,lNr can be use free. (get record with DL_dlRec__dli())) 
      OpenGL-index starts at DL_base_mod; 
   

  Modify Temporary-Dynamic-objects:
    overwrite existing record with DL_SetInd() setting DL_perm_ind

  Remove Temporary-Dynamic-objects:
    GL_Get_DLind - get DL-index (for remove all following DL-objects later)
    GL_Delete    - remove all following DL-objects



//================================================================
Temporary-Object                     GR_OTYP_TEMP
//================================================================

    have no Source-record, no DB-record, no DL-record, only a GL-record.
    cannot be hilited, limited nr of objs (30)
    selection returns typ=Typ_dynSym and the OpenGL-index (0-DL_base_font1)

  Create Temporary-objects:
    GR_temp_*() - display obj;  with a index 1 - (DL_base_font1 - 1)
      OpenGL-index is from 0 to DL_base_font1
      set a fixed index with DL_temp_ind

  Modify Temporary-objects:
    overwrite existing record with setting DL_temp_ind

  Remove Temporary-objects:
    GL_temp_del_1      // remove one
    GL_temp_del_all    // remove all

  Examples:
    fixed index: use #define GR_TMP_I0 8  /  DL_temp_ind = GR_TMP_I0;
    see cre_tmp_1 in ../APP/DemoPlugin_Create.c


//================================================================
ApplicationObjects

    used for plugin-specific objects
    use DisplayList-records with type Typ_APPOBJ
    can be seen / created / modified in plugins ../dox/ApplicationObjects.dox

ApplicationData

    used for plugin-specific data
    can be created / modified in plugins ../dox/ApplicationData.dox



//================================================================
//================================================================
Workflow:

  Creation of permanent objects:
    // APT_obj_stat  0=permanent, 1=temporary (workmode, do not store in DB, DL)
    DB_store_obj (... &dbi);
      // dbi: 0=DO-NOT-STORE-parent; -1=get-next-free-dynamic; >0=store-perm.
      DB_StorePoint (&dbi, ..)     // store obj in perm|dyn DB
        if(Ind < 0) DB_GetDynInd
        DB-pt=pt;
    GR_perm_*
      DL_StoreObj (.. dbi ..)     // create|update DL-record
        // if(DL_perm_ind < 0) get next-free-DL-index
        // else use dli = DL_perm_ind (modify-existing-DL-rec)
        // write DL-record
        // retCod = DL-index


  Creation of permanent-dynamic-objects:
    AP_add_*



//----------------------------------------------------------------
Functions to Create / Display grafic objects:

AP_add_*     add obj permanent into DB, add DL-record, add GL-record.   
GR_perm_*    add DL-record, add GL-record.
GR_tDyn_*    add DL-record, add GL-record.                    
GR_temp_*    add GL-record
GL_set_*     write GL-record


//----------------------------------------------------------------
GR_perm_*          // store permanent, without store in DB.
  GL_DrawLn_Ini    // GL_fix_DL_ind, glNewList glCallList(col+lineTyp+thick)
  GL_set_*         // add obj into open GL-record
  GL_set__ ();     // close GL-record


//----------------------------------------------------------------
GR_tDyn_*          // display Temporary-Dynamic-obj, unlim. nr
  GL_tDyn_init
    DL_dli_get     // get fixed or next free DL-index and GL-index
    DL_set__       // create DL-record
    GL_list_open   // glNewList
  GL_att_cv|GL_att_sym
  GL_set_*         // create GL-object
  GL_list_close    // close GL-record

//----------------------------------------------------------------
GR_temp_*          // display temporary-obj      (max 30)
  GL_temp_init     // get GL-index for temporary storage
    DL_temp_init   // glNewList
  GL_att_cv|GL_att_sym
  GL_set_*         // add obj into open GL-record
  GL_list_close    // close GL-record





================================================================== \endcode */}
void INF_OPERS(){        /*! \code

OPERS_PERM   open GL-list for permanent/dynamic-obj
OPERS_TDYN   open GL-list for temporary-dynamic-obj
OPERS_TEMP   open GL-list for temporary-obj
OPERS_CLOSE  close GL-list after adding obj
OPERS_NONE   do not open or close GL-list



================================================================== \endcode */}
void INF_GR_attrib(){        /*! \code

INF_COL_PT      set color for point;  GL_att_pt (ATT_PT_HILI);
INF_COL_SYMB    set symbol-color; GL_att_sym (ATT_COL_HILI);
INF_COL_CV      set color/thickness/linetype for curves; GL_att_cv  (Typ_Att_hili1);
                GL_att_su1 ?
                GL_att_OnTop_set

INF_Hilite_attribute
INF_Hide_attribute
INF_Pick_attribute
INF_Linetyp_attributes
INF_Color_attributes
INF_Texture_attributes
INF_permanent_attributes


See also INF_GL_attrib


//----------------------------------------------------------------
All grafic objects have states:
- normal
- hilite (points and curves thick,red; surfaces bright white)
- dimmed (all objects dimmed gray)

Surface can have states:
- normal (shaded)
- textured
- transparent
- transparent textured  (not implemented)
- symbolic (not shaded, curves along boundaries indicating surface)
- tesselate (store as triangles, ignore texture,color,.. - only for intersections)

See INF_GL_attrib


//----------------------------------------------------------------
Functions:
DL_dlRec__dli     get dispList-record;
DL_Redraw         redraw all (to display changes of dispList-records)



TODO: separate functions for Surface Shaded|Symbolic|tesselate.



================================================================== \endcode */}
void INF_GR_group(){        /*! \code
INF_GR_group        list of selected objects
../xa/xa_grp.c

(DL_Att) GR_ObjTab[].grp_1  ON|OFF    - if obj is in group

DL_grp1_set
DL_grp1_get
Grp_add__        // add obj to GrpTab
DL_grp1_copy     // copy all DL-obj with groupBit ON --> GroupList

Del_grp__        // delete all objs in group

Grp_init
Grp_Start




================================================================== \endcode */}
// EOF
