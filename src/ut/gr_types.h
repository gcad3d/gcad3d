/* func_types.h

Allgemeines Userinterface.


Korr:
2002-02-20 ../gr/ut_GR.h zu. RF.
2001-06-14 Neu zu. RF.

*/

// #define STAT_deleted       0
// #define STAT_normal        1


// attributes for lines,curves
#define Typ_Att_PT         0   // jellow
#define Typ_Att_def        1   // blue
#define Typ_Att_go         2
#define Typ_Att_rp         3
#define Typ_Att_Fac        4
#define Typ_Att_Fac1       5
#define Typ_Att_Fac2       6
#define Typ_Att_Symb       7
#define Typ_Att_hili       8  // white
#define Typ_Att_hili1      9  // red, thick=3
#define Typ_Att_dim       10
#define Typ_Att_top1      11
#define Typ_Att_top2      12

#define SYM_TRI_S        130   // Bitmap tringle small
#define SYM_STAR_S       131   // star, small
#define SYM_CIR_S        132   // Bitmap circle small
#define SYM_TRI_B        133   // Bitmap tringle big

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

// colors for Typ_PT:
#define ATT_PT_BLACK       0
#define ATT_PT_HILI        1
#define ATT_PT_DIMMED      2
#define ATT_PT_GREEN       3
#define ATT_PT_YELLOW      4

// colors for Typ_SymB:
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
