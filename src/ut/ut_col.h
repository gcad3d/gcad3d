/* ut_col.h


*/

/// *ColRGB from *int:
#define COL_INT32(i32) ((ColRGB*)(i32))


/// *int from *ColRGB
#define INT32_COL(col) ((INT_32*)(col))


/// Col_DL_Att          get pointer -> Color out of DL_Att     INLINE
ColRGB* Col_DL_Att (DL_Att *dla);
#define Col_DL_Att(dla) ((ColRGB*)&((dla)->iatt))


/// Col_set_vsym        set style, symbolic|shaded
void Col_set_vsym (ColRGB* col, int styl);
#define Col_set_vsym(col,styl) ((*(col)).vsym = (styl))


// EOF
