

typedef struct {int ip1, ip2; Point p1, p2; int typ;}    TypTsuSur;
// keeps data for one tesselated surface;
// ip1, ip2   von-bis Pointer
// p1,  p2    ??
// typ        ??
// size = 60



int TSU_tr_init_ (ObjGX *oxi);

int TSU_box_Draw1 (int ptNr, Point *p2Tab, char dir, Vector *vc1);

double TSU_uOff();



//================================================================
