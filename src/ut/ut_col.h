/* ut_col.h


*/

// ColRGB from int:
#define COL_INT32__(i32) (*((ColRGB*)&i32))


// *ColRGB from *int:
#define COL_INT32P(i32) ((ColRGB*)(i32))

// *char from *int (&att):
#define CHAR_ICOL(i32) ((unsigned char*)(i32))


// *int from *ColRGB
#define INT32P_COL(col) ((INT_32*)(col))

// int from *ColRGB
#define INT32I_COL(col) (*(INT_32*)(col))


// COL_DL_ATT          get pointer -> Color out of DL_Att     INLINE
// *ColRGB from *DL_Att
ColRGB* COL_DL_ATT (DL_Att *dla);
#define COL_DL_ATT(dla) ((ColRGB*)&((dla)->iatt))


// COL_ISTYL        set style, symbolic|shaded
// *ColRGB from (int)style // 
void COL_ISTYL (ColRGB* col, int styl);
#define COL_ISTYL(col,styl) ((*(col)).vsym = (styl))

// COL_TRANSP       set transparency; 0=not, 1=25% transparent, 2=50%, 3=75%
void COL_TRANSP (ColRGB* col, int transp);
#define COL_TRANSP(col,transp) ((*(col)).vtra = (transp))


int UTcol__3i (ColRGB *col1, int cr, int cg, int cb);
int UTcol__3db (ColRGB *col1, double d1, double d2, double d3);
int UTcol_dump (ColRGB *col, char *sInf);

// EOF
