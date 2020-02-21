/* ../inf/inf_GL.c



================================================================== */
void INF_GL__ (){        /*! \code

INF_GL_events


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
// eof
