/* ../inf/inf_GL.c



================================================================== */
void INF_GL__ (){        /*! \code

INF_GL_events
INF_GL_surfaces
INF_GL2D__
INF_GL_attrib


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
_fac     n faces (Fac3 = indexed triangle);    GL_set_nfac

//----------------------------------------------------------------
- ifac   n indexed faces with vectors;         GL_set_nifac



================================================================== \endcode */}
void INF_GL2D__ (){        /*! \code

INF_GL2D_buttons__    2D-OpenGL-buttons, Vector/PlaneSelector 


-------------------------------------------


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

GL_cen
GL_eye_upd
GL_eye_upd     // update GL_eye_pln and GL_angZ,GL_angX

GL_Scale              user-scale
GL2D_Scale            scale for GL2D (always same size)




Functions:
GL_Draw_sym_ang       display symbolic angle (GL-2D)
GL_DrawAngA           draw angle with arrowhead
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
GLBT_but_disp      display all defined 2D-buttons
GLBT_sel__         analyze selection of 2D-button or obj of Vector/PlaneSelector


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
// eof
