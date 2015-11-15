/* ut_UI.h

Allgemeines Userinterface.


Korr:
2002-02-20 ../gr/ut_GR.h zu. RF.
2001-06-14 Neu zu. RF.

*/

#define STAT_deleted       0
#define STAT_normal        1


/* -> ../gui/gui_types.h
#define UI_FuncInit       90
#define UI_FuncInit1     191
#define UI_FuncInit2     192
#define UI_FuncInit3     193
#define UI_FuncClear     194
#define UI_FuncGet        91
#define UI_FuncSet        92
#define UI_FuncDisable    93
#define UI_FuncEnable     94
#define UI_FuncFocus      95
#define UI_FuncPrev       96
#define UI_FuncNext       97
#define UI_FuncWork       98
#define UI_FuncKill       99
#define UI_FuncOK        100
#define UI_FuncCancel    101
#define UI_FuncExit      102
#define UI_FuncHelp      103
#define UI_FuncNO        104

#define UI_FuncInsert    110
#define UI_FuncModify    111
#define UI_FuncDelete    112
#define UI_FuncUndo      113

#define UI_FuncUCB      1000         // Usercallbacks; 1000-1999
#define UI_FuncUCB1     1001
#define UI_FuncUCB2     1002
#define UI_FuncUCB3     1003
#define UI_FuncUCB4     1004
#define UI_FuncUCB5     1005
#define UI_FuncUCB6     1006
#define UI_FuncUCB7     1007
#define UI_FuncUCB8     1008
#define UI_FuncUCB9     1009
#define UI_FuncUCB10    1010
#define UI_FuncUCB11    1011
#define UI_FuncUCB12    1012
#define UI_FuncUCB13    1013
*/

// auch Activate, Deactivate usw ...
// #define GUI_FuncReset   98
// #define GUI_QueryActive 94
// #define GUI_QueryState  95



#define QUERY             -1

#define ALL_OBJS    (long)-1




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

#define FUNC_Pan           1
#define FUNC_Rot           2
#define FUNC_RotCen        3

#define FUNC_ViewTop       4
#define FUNC_ViewFront     5
#define FUNC_ViewSide      6
#define FUNC_ViewIso       7
#define FUNC_ViewReframe   8
#define FUNC_ViewZoomBox   9
#define FUNC_ViewReset    10
#define FUNC_DispWire     11
#define FUNC_DispRend     12
#define FUNC_DispHid1     13
#define FUNC_DispHid2     14
#define FUNC_DispHid3     15

#define FUNC_Hili        870
#define FUNC_Dim         860
#define FUNC_Show        880
#define FUNC_Pick        890

#define FUNC_Idle        990


//============================================================


// textured or not; 0=normal (not textured), 1=textured.
#define IF_TEXURE(att) ((ColRGB*)&(att))->vtex & AP_stat.texture

void UI_AP (int func, int widgetID, void* data);




//============ EOF ============= 
