// utility-functions for Gtk-Toolbox

typedef struct {
  int  nrVal;      // nr of values per line
  char **pTab;     // table of <nrVal> pointers; strings; for out and in.
  char delVal;     // value-delimiter-char; comma or semicolon
  } CSV_struct;



void OS_Wait (int millisecs);
int OS_checkFilExist (char* filnam, int mode);
int OS_file_delete (char *fNam);
int OS_get_dialog ();
  char* OS_get_ico_dir ();
  char* OS_get_tmp_dir ();
int OS_file_sig_cre (int mode, void *data);
int OS_file_sig_wait (int mode, void *data);
long OS_FilSiz (char* filnam);

char* UTX_CleanCR (char* string);
int UTX_endDelChar (char* txtbuf, char chr);
void UTX_str_file (char* txbuf, long *fSiz, char *fnam);
int UTX_wrf_str (char *fnam, char *txt);
int UTX_tmp_file (char *fBuf, long *fSiz, char *fNam);

int MSG_get_1 (char *msg, int msgSiz, char *key, char *fmt, ...);
int MSG_err_1 (char *key, char *fmt, ...);
int MSG_pri_1 (char *key, char *fmt, ...);
char* MSG_get_str (char *key);

void TX_Print    (char *, ...);
void TX_Error    (char *, ...);
void TX_Write    (char*);


// EOF
