
// prereq: xa.h (AP_stat, APP_act_typ, APP_OBJ_NAM)
//         extern APP_act_typ



// PRC_is_active      returns 1 if process is active; else 0
// usage:  if(PRC_IS_ACTIVE) {   // process is active ..
#define PRC_IS_ACTIVE ((AP_stat.APP_stat)&&(APP_act_typ==2))


// ../xa/xa_prg.c ../xa/xa_proc.c ../xa/xa_plu.c
char* PLU_appNamTab_get (int iNam);


// EOF
