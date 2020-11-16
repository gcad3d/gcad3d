/* func_types.h

Allgemeines Userinterface.


Korr:
2002-02-20 ../gr/ut_GR.h zu. RF.
2001-06-14 Neu zu. RF.

*/

// #define STAT_deleted       0
// #define STAT_normal        1


#ifdef globTag
void INF_ATT_CV (){}
#endif
// attributes for lines,curves;
// for data see INF_COL_CV() or file ~/gCAD3D/cfg/ltyp.rc
// GL_att_cv
#define Typ_Att_def        0   // black
#define Typ_Att_blue       1   // blue
#define Typ_Att_dash__     2   // black dashed
#define Typ_Att_dash_dot   3   // black dash-dot
#define Typ_Att_Fac        4   // red
#define Typ_Att_dash_long  5   // blue thick2
#define Typ_Att_Fac2       6   // gray
#define Typ_Att_Symb       7   // yellow thick3
#define Typ_Att_hili       8   // green thick3 (symbolic surfs)
#define Typ_Att_hili1      9   // red thick=3 
#define Typ_Att_dim       10   // dimmed 1 (unlimited lines)
#define Typ_Att_top1      11   // black thick4
#define Typ_Att_top2      12   // red dashed thick2 (parent-obj's)



#ifdef globTag
void INF_BITMAP_SYMBOLS (){}
#endif
// bitmapsymbols for GL_Disp_symB color: GL_att_sym
// GL_set_symB GR_tDyn_symB__ GR_temp_symB
#define SYM_TRI_S        130   // Bitmap tringle small
#define SYM_STAR_S       131   // star, small
#define SYM_CIR_S        132   // Bitmap circle small
#define SYM_SQU_S        133   // Bitmap square small
#define SYM_TRI_B        134   // Bitmap tringle big
#define SYM_SQU_B        135   // Bitmap square big

// vector-symbols
#define SYM_VEC          139   // das Vektorsymb (nicht skaliert)

#define SYM_AXIS         140   // first vector-symbol; Axis mit X,Y,Z-Char's
#define SYM_AXIS1        147   // Axis ohne Buchstaben (Sym.Rotcen)
#define SYM_SQUARE       142
#define SYM_PLANE        145
#define SYM_CROSS        143   // PosCross
#define SYM_TRIANG       141
#define SYM_CROSS1       144   // Schere
#define SYM_ARROH        146   // arrow - 2D-head only
#define SYM_ARROW        148   // arrow
#define SYM_ARRO3H       138   // arrow - 3D-head only
#define SYM_LENGTH       149   //

#define SYM_CLIPBOX      150   // internal use, Vektor; unused ..



#ifdef globTag
void INF_COL_PT (){}
#endif
// color of point(-symbols); value = thickness
// GL_att_pt
// GL_InitPtAtt
#define ATT_PT_BLACK       0   // thick 5
#define ATT_PT_HILI        1   // thick 5
#define ATT_PT_DIMMED      2   // thick 5
#define ATT_PT_GREEN       3   // thick 5
#define ATT_PT_YELLOW      4   // thick 9


#ifdef globTag
void INF_COL_SYMB (){}
#endif
// color of symbols (text, ..) and (tesselated) surfaces
// Definition: GL_col_tab in ../gr/ut_GL.c
// GL_att_sym
#define ATT_COL_BLACK      0
#define ATT_COL_RED        2
#define ATT_COL_GREEN      3
#define ATT_COL_BLUE       4
#define ATT_COL_YELLOW     5
#define ATT_COL_MAGENTA    6
#define ATT_COL_CYAN       7
#define ATT_COL_WHITE      8
#define ATT_COL_HILI       9
#define ATT_COL_DIMMED    10

// thick symbols
#define ATT_COL_T_BLACK     16
#define ATT_COL_T_RED       18
#define ATT_COL_T_GREEN     19
#define ATT_COL_T_BLUE      20
#define ATT_COL_T_YELLOW    21
#define ATT_COL_T_MAGENTA   22
#define ATT_COL_T_CYAN      23
#define ATT_COL_T_WHITE     24
#define ATT_COL_T_HILI      25
#define ATT_COL_T_DIMMED    26




// linetypes
#define LTYP_full          0
#define LTYP_dashLong      3
#define LTYP_dashShort     2
#define LTYP_dashDot       1


//============================================================


// textured or not; 0=normal (not textured), 1=textured.
#define IF_TEXURE(att) ((ColRGB*)&(att))->vtex & AP_stat.texture

void UI_AP (int func, int widgetID, void* data);




//============ EOF ============= 
