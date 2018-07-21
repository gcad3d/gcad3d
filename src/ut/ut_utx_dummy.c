/* ../ut/ut_utx_dummy.c
provides necessarey dummy-funcs and var for ../ut/ut_txt.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ../xa/.xa.h
char AP_lang[4];           // en or de
char APP_act_nam[128];     // name of last script- or plugin-program







//================================================================
// ../xa/xa_main.c

//================================================================
  char* OS_get_bin_dir () {
//================================================================
/// OS_get_bin_dir           returns path of executables, $gcad_dir_bin
  
  return "/";
  
} 
  
//================================================================
  char* OS_get_bas_dir () {
//================================================================
/// OS_get_bas_dir           returns path of ?
  
  return "/";

} 

//================================================================
  char* OS_get_ico_dir () {
//================================================================
/// returns directory for icons (with closing '/')  <gcad_dir_bin>icons/
  
  return "/";  
  
}
  
//================================================================
  char* OS_get_doc_dir () {
//================================================================
/// OS_get_doc_dir           returns path of docu
           
  return "/";

} 
  
//================================================================
  char* OS_get_cfg_dir () {
//================================================================
/// returns confDir (with closing '/')  <gcad_dir_local>xa/

  return "/";

}

//================================================================
  char* OS_get_loc_dir () {
//================================================================
/// returns confDir (with closing '/')  <gcad_dir_local>xa/

  return "/";

}


//================================================================
char* OS_get_tmp_dir () {
//================================================================
  static char s1[]="/mnt/serv1/Devel/gcad3d/gCAD3D/tmp/";
  return s1;
}



//================================================================
// ../gui_gtk3/gtk_base.c
//=====================================================================
  void GUI_Tip (char* txt) {
//=====================================================================
  printf("***** GUI_Tip |%s|\n",txt);
  return;
}

//================================================================
  int GUI_ck_version (int vMaj, int vMin) {
//================================================================
  printf("***** GUI_ck_version %d %d\n",vMaj,vMin);
  return 0;
}


// ../gui_gtk3/gtk_dlg_std.c
//================================================================
  int GUI_MsgBox (char* text) {
//================================================================
  printf("***** GUI_MsgBox |%s|\n",text);
  return 0;
}



//================================================================
// ../xa/xa.c

//=================================================================
  int AP_errStat_set (int stat) {
//=================================================================
  printf("***** AP_errStat_set %d\n",stat);
  return 0;
}




//================================================================
// ../ut/ut_uti.c

//================================================================
  int UTI_round_i2b (int ii) {
//================================================================
/// \code
/// UTI_round_i2b             round integer to byte (back: UTI_round_b2i)
///  0,1 -> 0; 2 -> 1; 3,4 -> 2; 5-8 -> 3; 9-16 -> 4; 17-32 -> 5; .. 
///   Purpose: save Integers in a single byte, rounding up.
///     UTI_round_i2b(20)    -> 5;     // all values from 17 to 32 give 5;
///     UTI_round_b2i(5)     -> 32
/// \endcode

//  ii = 10000;
//  printf(" ii=%d, i2b=%d; b2i=%d\n",ii,
//           UTI_round_i2b(ii),UTI_round_b2i(UTI_round_i2b(ii)));


  int   ib = -1;

  if(ii < 2) return 0;

  ii *= 2;
  --ii;

  while(ii) {ii /= 2; ++ib;}

  return ib;

}


//================================================================
  int UTI_round_b2i (int i1) {
//================================================================
/// \code
/// UTI_round_b2i         make integer from byte (back from UTI_round_i2b)
/// 0 -> 0; 1 -> 2; 2 -> 4; 3 -> 8; 4 -> 16; 5 -> 32; .. 8=256; .. 10=1024 ..
/// \endcode

  int ii=1;

  if(i1) {
    while(i1) {--i1; ii *= 2;}
  } else return i1;

  return ii;

}


// EOF
