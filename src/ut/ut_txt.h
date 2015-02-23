/* ut_txt.h


2002-02-25 term_buf u term_anz von ut_txfil.h hierher. RF.

*/



#define TXT_Typ_EOF       0
#define TXT_Typ_spec      1
#define TXT_Typ_text      2         /* nicht in " eingeschlossenen Texte */
#define TXT_Typ_num       3
#define TXT_Typ_string    4         /* in " eingeschlossenen Texte */






/// Anzahl Zeilentrennzeichen 2 (CRLF), or 1 (LF).
#define  term_anz 1
#define  term_buf "\n"

// #define  term_anz 2
// #define  term_buf "\r\n"


/// FilenamedelimiterChar
#ifdef _MSC_VER
#define  fnam_del '\\'
#define  fnam_del_s "\\"
#else
#define  fnam_del '/'
#define  fnam_del_s "/"
#endif




// ------------- inline functions ------------------------------------
#define UTX_Clear(str) str[0] = '\0'
/// clear string


/// get length of rest of string
#define UTX_Siz(sSiz,sStart0,sStart1) sStart0 + sSiz - sStart1
/// \code
///   sSiz     total size of string from sStart0 to end of string
///   sStart0  start of stringbuffer
///   sStart1  first free char in stringbuffer (space sStart0 - sStart1 is in use)
///   retCode  remaining space from sStart1 to end of string
/// \endcode


/// skip the active char and following blanks.    tp=(char**) !
#define UTX_skip_1bl(tp) {++(*(tp)); while(**(tp) == ' ') ++(*(tp));}


/// skip blanks (returns Position of first non-blank)
#define UTX_pos_skipLeadBlk(p1) while(*p1 == ' ') ++p1


/// find end of string ('\0')           p1 = UTX_pos_EOS(cString);
#define UTX_pos_EOS(p1) strchr(p1, '\0')

// -------------------------------------------------------------------
extern const char TX_NUL;





/*****************************************************************************/
/*                              PROTOTYPES                                   */
/*****************************************************************************/

  void  UTX_cp_word_2_upper  (char* txtout, char* txtin);
  char* UTX_cp_word_term     (char* txtOut, char* txtIn, char term);
  char  UTX_cp_word_t        (char *txtOut, char **cBuf);
  char* UTX_cp_word__        (char *txtOut, char *txtIn);
  int   UTX_cp_right         (char *sOut, char *sIn, int cNr);
  char* UTX_cp_expr          (char *cbo, char *cbi);

  int   UTX_ftyp_s (char *ftyp, char *cbuf, int mode);
  int   UTX_fnam_s (char *fnam, char *cbuf);
  int   UTX_fdir_s (char *fdir, char *cbuf);

  char  UTX_pos_del          (char**, char**);
  long  UTX_pos_del_next     (char *cBuf, long cPos);
  long  UTX_pos_del_prev     (char *cBuf, long cPos);
  char* UTX_pos_eos          (char*);
  char* UTX_pos_1n           (char*);
//void  UTX_pos_skipLeadBlk  (char *buf);
  char* UTX_pos_skipBrack    (char *cbuf);
  char* UTX_pos_skipBrack1   (char *cbuf);
  int   UTX_pos_skipTermWord (char **wNxt, char *cbuf);
  char* UTX_pos_skipWord     (char *cbuf);
  char* UTX_pos_skipWords    (int wNr, char *p1);
  char* UTX_pos_skipStr      (char *cbuf);
  char* UTX_pos_skipDeli1    (char *p1);
  int   UTX_pos_skip_int     (char **ps);

  int   UTX_ckc_Del1   (char c1);
  int   UTX_ckc_Del2   (char c1);
  int   UTX_ck_num_i   (char *s1);
  int   UTX_ck_num_f   (char **pOut, char *cbuf);
  int   UTX_ck_num_digNr (char **cpo, char *cpi);
  int   UTX_cmp_word_wordtab (char *wdtab[], char *wd1);

  char* UTX_2db_tx     (double *d1, double *d2, char *cbuf);
  double UTX_db_tx     (char **po, char *pi);

  int   UTX_add_i_u    (char[], int);
  int   UTX_add_db     (char outBuf[], double *db1);
  int   UTX_add_pt     (char outBuf[], void *pt1);
  int   UTX_add_fl_u   (char[], double);
  int   UTX_add_fl_u2  (char[], double, double);
  int   UTX_add_fl_u3  (char[], double, double, double, char chDel);
  int   UTX_add_fl_ua  (char[], double *da, int dNr, char chDel);
  int   UTX_add_fl_f   (char[], double, int);
  char* UTX_add_fl_1uf (char *outBuf, double d1);
  int   UTX_add_fl_10  (char[], double);
  int   UTX_add_fl_15  (char[], double);

  int   UTX_del_chr    (char *cbuf, char cx);
  int   UTX_del_foll0  (char strBuf[]);
  void  UTX_del_follBl (char *cbuf);
  int   UTX_del_FilTx  (char *dtxt, char *filNam);

  char* UTX_fgets      (char* line, int len, FILE* fp);
  int   UTX_fgetLine   (char *cbuf, int sizBuf, char *filNam, int lNr);
  int   UTX_fsavLine   (char *cbuf, char *filNam, int lnMaxSiz, int lNr);
  int   UTX_fRevL      (char *fno, char *fni);

  void  UTX_ins_s2     (char*, char*, int);

  char* UTX_CleanCR    (char* string);
  void  UTX_CleanSC    (char*);
  void  UTX_CleanAN    (char*);
  int   UTX_CleanCommC (char *cBuf);
  int   UTX_Clean_db   (char*);
  int   UTX_chg_chr1   (char newChr, char oldChr, char *cBuf);
  int   UTX_chg_str1   (long *datSiz,
                        void *datStart, void *insDat, void *delPos, long delSiz);
  int   UTX_chg_str2   (int sSiz, char *sDat,  char *sOld,  char *sNew);
  int   UTX_chg_nr     (char *txtOut, int iNr);
  int   UTX_chg_wordn  (char *sOut, char *wNew, int wNr, long lenMax);
  char* UTX_CleanBracks (char* txtbuf, char ch1, char chr2);
  int   UTX_endDelChar (char* txtbuf, char chr);

  int   UTX_get_word   (char** txtIn,char** w_next,char* txtOut,double* val,
                                                              char* deli);
  char* UTX_find_word1 (char *was, char *wo);
  char* UTX_find_Del1  (char *p1);
  char* UTX_find_strrstr (char *cbuf, char *str);
  int   UTX_strcmp_right (char *string, char *text);

  int   UTX_ck_casenChr(char *s1, char *s2, int cNr);

  char *UTX_memFind    (char *buf, char *str);
  char *UTX_memFind1   (char *buf, long bufSiz, char *str, long strSiz);
  char *UTX_memstr     (char* von, char* bis, char* sstr);
  int   UTX_insert1    (char *cbuf, long *bufSiz, char *auxBuf, long cPos);
  int   UTX_cut1       (char *cbuf, long *bufSiz, long cPos, long txLen);

  int   UTX_wTab_ck    (char *wTab[], char *w1);
  char** UTX_wTab_file (char *memspc, int memSiz, char *fnam);

  int UTX_wordnr       (char *cbuf);
  int UTX_setup_set    (char *fn, char *par, char *val);
  int UTX_setup_get    (char *cval, char *ctyp, char *fnam, int mode);
  int UTX_setup_modw   (char *fn, char *par, char *wNew, int wNr);
  int UTX_setup_decs   (char *s1, char **pv);

  int UTX_cat_file     (FILE *fpo, char *fnam);
  int UTX_dir_listf    (char *outFilNam, char *fPath, char *fNam, char *fTyp);
  int UTX_cnr_chr      (char *txt, char c1);

  int  UTX_str_file    (char* txbuf, long *fSiz, char *fnam);

/* ------------ eof ---------------- */
