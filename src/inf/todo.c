/* ../inf/todo.c
================================================================== */
void INF_TODO__ (){                   /*! \code

BUG_Select-sm-image

TODO:
INF_TODO_CTLG_1  add title "CATALOG: <activeCatalogName>" to select-catalog-window
TODO_FMT_MOCK_NAT mockup-native-format (Text)
TODO_mockup-native-format - .tess-files differnet between Unix - MS-Win
BUG_DXF_export_Mockup  -


================================================================== \endcode */}
void INF_TODO_undo_objs_plu (){        /*! \code
- check undo for objects created from plugins 

see UNDO_app__
see ../APP/DemoPlugin_Dialog.c

keep last DL-index, undo removes all following objs ?


================================================================== \endcode */}
void INF_TODO_SMPOS (){        /*! \code
../APP/tst_gmvo.c          Move all obj's of group



================================================================== \endcode */}
void INF_TODO_SMPOS (){        /*! \code
INF_TODO_SMPOS          "move subModelPosition"

OMN_CB_popup   MSG_move:
  UI_Tra__
    UI_Tra_Mod

GUI_menu_entry "submodelPosition"
UI_men__ "ModPos"

(GR_Sel_Filter == 16)

Files:
../xa/xa_tra.c   UI_WinTra__ UI_Tra__ ..




================================================================== \endcode */}
void INF_TODO_1 (){        /*! \code

Problem:
- trimmed line does not have a DB-obj (is not stored as dynamic line)
  - see eg CVTRM_basCv_trmCv
  - store as dynamic line ?
  - store in temp.workspace ?
  - change line into struct with 2 pointers into points-DB ?




================================================================== \endcode */}
void INF_TODO_BRW (){        /*! \code

BROWSER:

TODO:
- does not display deleted objects after refresh (eg after MAN)
- so cannot restore deleted vectors !

TODO:
- must display after refresh (eg after MAN) all typeRows with deleted objects
  if eg the only existing line is deleted, then no typRow Line exists;
  - so the line cannot be restored ...


================================================================== \endcode */}
void INF_TODO_GTK (){        /*! \code

../gui_gtk3/gtk_multichoice.c:651:3: warning: 
 ‘gtk_menu_popup’ is deprecated:
Use:
 gtk_menu_popup_at_widget,
 gtk_menu_popup_at_pointer,
 gtk_menu_popup_at_rect
instead 
   gtk_menu_popup (GTK_MENU(menu), NULL,NULL,NULL,NULL,0,0);


================================================================== \endcode */}
void INF_TODO_MDLFN_1 (){        /*! \code

Fixed.


================================================================== \endcode */}
void INF_TODO_IMP_OBJ (){        /*! \code

./do OBJ_U/myHome3.obj
/mnt/serv2/devel/cadfiles/obj/myHome3.obj
- keine Farbe ... aus /mnt/serv2/devel/cadfiles/obj/myHome3.mtl
- wird aber gelesen (GA_creRec ?) und (nur mit tst_imp_obj) angezeigt;
- werden die Attributes nicht gespeichert ?

obj_read__ |/mnt/serv2/devel/cadfiles/obj/myHome3.obj|
  obj_r_mtllib



================================================================== \endcode */}
void BUG_Select-sm-image (){        /*! \code

//   if image is in subModel, position and extents cannot be found in DL_txtSelect
//   possible solutions:
//      - make (selectable) outline; useless if image covers whole window
//      - store position and extents in ModelBas ?
//      - use texture TexBas ?




================================================================== \endcode */}
void TODO_FMT_MOCK_NAT (){        /*! \code
mockup-native-format (Text)

existing codes:
    A#=RCIR ..   // triangle

  obsolete "SECTION MESH" and "SECTION PTAB" (indexed-surfaces)
    - ev muss dazu auch PRCV ausgegeben werden (siehe "SECTION PTAB")
    - Texturinfos (dzt attribute)

  see stp obj



SECTION PTAB _A1
ptnr=2500


SECTION MESH _A2
faces=4802 edgelines=0


PTAB = points
edgelines = polygonal boundaries;
faces = triangles (3 indexes)

Andere Variante eg FcNbP (faces, neighbour-faces, status-of-points)
- TessStru von MSHIG


MSH_asav_fTabf()




================================================================== \endcode */}
// eof
