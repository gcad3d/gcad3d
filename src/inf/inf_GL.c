/* ../inf/inf_GL.c



================================================================== */
void INF_GL__ (){        /*! \code

INF_GL_events
INF_GL_surfaces
INF_GL2D__
INF_GL_attrib
INF_GL_coords
INF_GL_V1             info ../gr/ut_GL.c

INF_GL_disp_point
INF_GL_disp_curve
INF_GL_disp_symbols   N = LDRS ..
INF_GL_disp_ObjNames  disp objID
INF_GL_Shade_OFF      function surfaces - display wireframe not shaded
INF_GL_ObjNames_ON    display objIS's (names) for all existing objects



//   GL_mode_draw_select =
//     GR_MODE_DRAW      0   normal display
//     GR_MODE_SELECT    1   selection is active
//     GR_MODE_FEEDBACK  2   get data -> FeedbackBuffer (eg for print)


Functions:
GL_Redraw             // redraw the OpenGL-list
AP_print__
  GL_FeedGet          // get FeedbackBuffer from OpenGL
    GL_Feedback
  AP_print_work2      // write FeedbackBuffer-data -> file  <tempDir>/print.tmp

Files:
../gr/ut_GL.c



================================================================== \endcode */}
void INF_GL_events (){        /*! \code

UI_GL_draw__
UI_GL_mouse__
UI_GL_keys__
UI_GL_move__

GL_Redraw



================================================================== \endcode */}
void INF_GL_surfaces (){        /*! \code

Types of gl-surfaces:

//----------------------------------------------------------------
_tria    triangles;                            GL_set_ntri

//----------------------------------------------------------------
_fan     fan of triangles;                     GL_set_fan

//----------------------------------------------------------------
_strip   stripe of triangles;                  GL_set_strip*

//----------------------------------------------------------------
_patch   fan or stripe;                        GL_set_patch

//----------------------------------------------------------------
_ipatch  indexed fan or stripe;                GL_set_ipatch

//----------------------------------------------------------------
_fac     n faces (Fac3 = indexed triangle);    GL_set_nfac_V1

//----------------------------------------------------------------
_nifac   n indexed faces with vectors;         GL_set_nifac_V1



================================================================== \endcode */}
void INF_GL2D__ (){        /*! \code

INF_GL2D_buttons__    2D-OpenGL-buttons, Vector/PlaneSelector 




GL_Scr_Siz_X        Size of the grafic-window (=Viewport) in ScreenCoords
GL_Scr_Siz_Y


/// GL_eyeX goes from the screenCenter to the eye
/// GL_eyeZ goes up - always vertical, parallel to the window outline
/// GL_eyeY always is horizontal, parallel to the window outline

  // eye-Punkt errechnen; GL_eyeX zeigt von GL_cen -> eye
  // GL_eyeX == umgekehrte Sichtlinie !!
GL_eye_pln and GL_angZ,GL_angX


DL_Ind_Cen2D          DL-record to set to 2D-screenCenter  DL_Set_Cen2D
                      // better use glLoadIdentity - see GL_Draw_sym_ang
DL_Ind_Scl2D          DL-record rotate (see DL_Set_Scl2D)
                      // DL_Set_Scl2D - func rotate into 2D-horizontal position
                      // better use glLoadIdentity - see GL_Draw_sym_ang
DL_Ind_ScBack         DL-record scale for GL2D (always same size) see GL_ScalBack)
                      // better use glLoadIdentity - see GL_Draw_sym_ang

GL_cen                centerpoint of the Viewport (in UserCoords)
GL_eye_upd
GL_eye_upd     // update GL_eye_pln and GL_angZ,GL_angX

GL_Scale              user-scale
GL2D_Scale            scale for GL2D (always same size)




Functions:
GL_Draw_sym_ang       display symbolic angle (GL-2D)
GL_DrawSymVTL         display vector-symbol with true length at 2D-ScreenCenter
GL_DrawSymVX          display plane / axisSystem [with x,y,z-characters
GR_img_get_obj        ??



================================================================== \endcode */}
void INF_GL2D_buttons__ (){        /*! \code

2D-OpenGL-buttons, Vector/PlaneSelector 

Typ_FncVC1         next DB-vector
Typ_FncVC2         previous DB-vector

GLBT_ori           GL2D-position of PlaneSelector


Functions:
sele_set_add       activate 2D-buttons
GLBT_but_disp      display all defined 2D-buttons
GLBT_sel__         analyze selection of 2D-button or obj of Vector/PlaneSelector


-------------------------------------------
Howto add a new 2D-button:
- add case and a call to sele_set_icon into sele_set_add; eg:
    case Typ_modUnlim:
      sele_set_icon (&i2Dbutts, GR_reqTyp);

- set text into GL2Dtxt1; eg add "UNL"
  check if text can be supplied from AP_src_typMod()


================================================================== \endcode */}
void INF_GL_coords (){        /*! \code

  printf(" width of grafic-window in screenCoords %f\n",GL_Scr_Siz_X);
  printf(" heigth of grafic-window in screenCoords %f\n",GL_Scr_Siz_Y);
    // ATT: right side of grafic-window is removed by browser window;
    //   therefore GL_cen is not center of window !
    //   Low Left corner always has screenCoords 0,0

  // screenCenter in userCoords
  DEB_dump_obj__ (Typ_PT, &GL_cen, " GL_cen");

    printf(" GL_Scale=%f\n",GL_Scale);
    printf(" GL2D_Scale=%f\n",GL2D_Scale);

  // vectors along grafic-window-boundaries
  DEB_dump_obj__ (Typ_VC, GL_eyeY, " GL_eyeY-hor");   // horizontal
  DEB_dump_obj__ (Typ_VC, GL_eyeZ, " GL_eyeZ-vert");  // vertical
  DEB_dump_obj__ (Typ_VC, GL_eyeX, " GL_eyeX-up");    // vector up to eye (normal)


  // get userCoords of point in screenCoords
  GL_ptUc_ptSc (&pt2, &UT3D_PT_NUL);  // get pt2 = low-left-corner in userCoords

  // get screenCoords from userCoords
  GL_ptSc_ptUc


  GL_View_set   // restore view from data
  GL_View_get   // get all parameters for view



================================================================== \endcode */}
void INF_GL_attrib (){        /*! \code

-------------------------------------
GL_att_OnTop__
  glDisable (GL_DEPTH_TEST);
  glDepthFunc (GL_ALWAYS);

GL_att_OnTopOff  (normal)
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);


-------------------------------------
GL_att_sur   (normal)
  glDisable (GL_BLEND);
  glEnable (GL_LIGHTING);

GL_ColSet
  SURF:
    glColor3ubv

  SURF_SYMBOLIC: (normal)
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);   // draw lines, not faces !?

  SURF_TRANSPARENT: (normal)
    glEnable (GL_BLEND);
    glColor4ubv                   // add transparency


SURF_DIMMED:
  init: GL_DefineDisp (FUNC_DispDimm__, 0);
  glDisable (GL_COLOR_MATERIAL); // ?
  exit: GL_DefineDisp (FUNC_DispDimm_ex, 0);

  SYMBOLIC-?
  TRANSPARENT-?

SURF_HILITE:
  glEnable (GL_BLEND);
  glDisable (GL_LIGHTING);
  glClear (GL_DEPTH_BUFFER_BIT);  // damit wirds ganz oben gezeichnet ..


-------------------------------------
GL_att_cv   (normal)
  glDisable (GL_LIGHTING);
  glCallList (DL_base_LnAtt + cvTyp);  // color,lineType,thick

GL_InitNewAtt // 0 = create / restore attribute-record
              // 1 = change this attribute to hilited
              // 2 = change this attribute to dimmed

  glEnable (GL_LINE_STIPPLE);   // dash-dot
  glLineStipple (LtypTab1[typ], LtypTab2[typ]);
  glLineWidth
  glColor3fv


-------------------------------------
GL_att_pt    (normal)   GL_InitPtAtt
  glDisable (GL_LIGHTING);
  glCallList (DL_base_PtAtt + ptTyp); // color,size

GL_InitPtAtt // -2=hilite; -4=dimmed; -1=reset-from-hilite; 0=reset-from-hilite

  glPointSize
  glColor3fv



TODO: make GL-list-record eg for surface-attribute;  change it in GL_Redraw
  according to normal|hilite|dim.
  Same for points, curves ..
  


================================================================== \endcode */}
void INF_GL_disp_point (){        /*! \code

see INF_COL_PT  // color and thickness of point; five records, 0-4
- do not use pointAttributes; use symbols ... see ?

#define DL_base_PtAtt 195    - 5 GL-records (195-199) - ../gr/ut_GL.h
  0 = black, stdThick
  2 = dimmed (ATT_COL_DIMMED), stdThick
  4 = hilite (ATT_COL_YELLOW), size hiliThick

Display/PointDisplay ON
UI_men__ |ckb_ptDisp|     sets APT_dispPT;  ON or OFF
  DL_hide_unvisTypes (0);
    GL_InitPtAtt

GL_InitPtAtt   // load GL-records DL_base_PtAtt (195-199)
  - glPointSize stdThick or hiliThick
  - set GL-record (DL_base_PtAtt + pTyp) - color and size of point

GR_perm_pt
  GR_set_obj ( form=Typ_PT )
    GL_att_pt
      glCallList (DL_base_PtAtt + ptTyp);    // set col & size



================================================================== \endcode */}
void INF_GL_disp_curve (){        /*! \code

- set color, stipple, thickness

- MAN command ATTL
- interactive function Modify/Linetyp
see INF_COL_CV   list of linetypes (color, stipple, thickness)
  - list is file ../../gCAD3D/cfg_Linux/ltyp.rc
  - Change / add attribute with DL_InitAttRec ();
    - or interactive function Standards/Linetypes
  - Reload default-attibutes with DL_InitAttTab ().

LN_WIDTH_FAT


Modify/Linetyp  - set new lintype for selected lines
UI_men__ |LtypMod|
  UI_WinLtypMod
UI_GR_Select_work1 - GR_Sel_Filter == 6
  GA_lTyp__               // modify lineType


GR_set_ocv                // create curve 
  GL_set_ocv GR_set_pcv
    GL_att_cv
      glCallList (DL_base_LnAtt + cvTyp);  // call GL-linetype-record
        // cvTyp = linetype from ltyp.rc)


GL_InitNewAtt          // change a GL-linetype-record;



================================================================== \endcode */}
void INF_GL_disp_symbols (){        /*! \code
CAD-Text/N symbol         ../../doc/html/CAD_TxtG_en.htm#F10
N = LDRS SymbolPosition [SymbolType] [Color] [EndpointVector]
SymbolType   SYM_TRI_S-SYM_SQU_B  ../ut/gr_types.h

APT_decode_ldrs             // create obj AText T_LDRP

GR_perm_txt__
  GR_set_txt__
    GL_att_sym            // color width
    GL_set_TxtSym
      GL_set_symB           // 0-3 bitmap-symbols, not oriented
        glCallList (symTyp)   // symTyp SYM_TRI_S - SYM_SQU_B
      GR_set_symV3          // 4,5 vectors
      GL_set_arrh2D         // 6   arrowhead 2D




================================================================== \endcode */}
void INF_GL_disp_ObjNames (){        /*! \code
display / hide objID's of all objs
Interctive function Display/ObjNames-ON

UI_men__ |ckb_nam|
  set int APT_disp_att - bit-0 = 1=disp-ObjNames; 0=not

GR_temp_att__
  GR_temp_att_1 (0);          // set ObjNames ON
    - open GL-record GR_TMP_I0; pack all ObjNames into this temp. record.

GL_temp_del_1 (GR_TMP_I0);  // set ObjNames OFF



================================================================== \endcode */}
void INF_GL_disp_ObjDirection (){        /*! \code
display / hide direction-arrow for all curves
Interactive function Display/ObjDirection ON

UI_men__ |ckb_dir1|
  set int APT_disp_att - bit-1 = 2=disp-ObjNames; 0=not

GR_temp_att__
  GR_temp_att_1 (1);             // set ObjDirection ON
    - open GL-record GR_TMP_IDIR; pack all ObjDirections into this temp. record.

GL_temp_del_1 (GR_TMP_IDIR);   // set ObjDirection OFF



================================================================== \endcode */}
void INF_GL_Shade_OFF (){        /*! \code
surfaces - display surfaces shaded or wireframe (triangle-boundaries)

GL_Redraw - calls the GL-record without preconditions 

GL_ColSet - called when creating surface or triangle
  setup GL-record - activate Color,transparency
  then call GL-record DL_shade_wire

GL_DefineDisp - called by user, call GL_atts_set_shade or GL_atts_set_wire
  GL_atts_set_shade set GL-record DL_shade_wire empty (GL_ColSet sets color, transp..)
  GL_atts_set_wire  set GL-record DL_shade_wire to dislay surface as black boundarylines




================================================================== \endcode */}
void INF_GL_ObjNames_ON (){        /*! \code
display objIS's (names) for all existing objects





================================================================== \endcode */}
// eof
