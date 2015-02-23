
void AP_obj_blank (char *ED_buf1);
int AP_obj_2_txt (char *ED_buf1, long bufLen, ObjGX *o1, long ind);
int AP_obj_2_apt (char *ED_buf1, long bufLen, ObjG *o1, void *data);
int AP_obj_2_txt_query (int *typ, long *ind);

int AP_obj_add_val (char *ED_buf1, double val);
int AP_obj_add_pt (char *ED_buf1, Point *pt1);
int AP_obj_add_pt2 (char *ED_buf1, Point2 *pt1);
int AP_obj_add_pt3 (char *ED_buf1, Point *pt1);
int AP_obj_add_ln (char *ED_buf1, int mode, Point *pt1, Point *pt2);
int AP_obj_add_ci_ (char *ED_buf1, Circ *ci1);
int AP_obj_add_ci1 (char *ED_buf1, Point *pc, double *rdc, Vector *vz);
int AP_obj_add_ci2 (char *ED_buf1, int mode, Circ *ci1);
int AP_obj_add_ci (char *ED_buf1, int mode, Circ *ci1);
int AP_obj_add_vc (char *ED_buf1, Vector *vc1);
int AP_obj_add_func1 (char *ED_buf1, int mode, double ang1);




//============== EOF ================
