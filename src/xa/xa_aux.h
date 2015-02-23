// import / export


typedef struct {long ind, trInd; unsigned char typ, form;
                unsigned stat01:1, stat23:1;
                unsigned fTyp:10, fInd:20, done:1, activ:1;}          ImpObj;
// unused: unsigned long ID; void *data;


void AP_ImportIges (char *off, int mode3d, char* fnam);
void AP_ImportDxf  (int mode, char *off, char* fnam);
void AP_ExportDxf  (char* fnam, int subVersion);
void AP_ExportIges (char* fnam);
void AP_ImportLwo(int mode, char *off, double scale, char* fnam);
void AP_Import3ds(int mode, char *off, double scale, char* fnam);

  int AP_appr (char *obj, char *ref, double tol, double lmin, int mode);


//=================== EOF ===========================
