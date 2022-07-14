// ../xa/mdl__.h
// ../xa/mdl__.c
// is included from ../xa/xa.h


#define SIZFNam    400         // full filename 
#define SIZMFNam   128         // filename without directory, without filetyp
#define SIZMFTyp   16          // filetyp ("dxf" "gcad" ..)
#define SIZMFSym   64          // symbolic-directoryname
#define SIZMFTot   256         // symbolic|safe-modelnames (symDir/fNam.fTyp), fDir


// stru_FN                decoded filename of cad-model
// symDir     symbolic name of directory;                               AP_mod_sym
//            may NOT have ending '/'; can be empty
//            can have extrapath; ed "Data/symEl1" . "symEl1" is extrapath;
// fDir       the full path without filename and filetyp;               AP_mod_dir
//            must have ending '/'
// fNam       the filename (no filetype); eg "unknown"                  AP_mod_fnam
//            can be empty; does not end with '.'
// fTyp       filetyp, eg "dxf" or "gcad"                               AP_mod_ftyp
//            can be empty;
// iTyp       integer-filetyp; eg Mtyp_DXF                              AP_mod_iftyp
typedef struct {char symDir[SIZMFSym],
                     fDir[SIZMFTot],
                     fNam[SIZMFNam],
                     fTyp[SIZMFTyp],
                     iTyp;
                }                                 stru_FN;
// see MDLFN_oFn_fNam
// see AP_mod_sym, AP_mod_dir, AP_mod_fnam, AP_mod_ftyp, AP_mod_iftyp
// TODO: add also AP_stat.subtyp into stru_FN


//================================================================
char* MDL_mNam_main ();
char* MDL_mNam_prim ();
char* MDL_mNam_iNam (int inm);
char* MDL_mNamf_prim ();
char* MDL_mNam_imr (int imr);
char* MDL_safNam_mnam (char *safNam, char *mnam);
char* MDL_typInf (int mTyp);



//================================================================
// MDL_MTYP_IS_MOCK        check if model is mockup (tess obj, stl, wrl)
#define MDL_MTYP_IS_MOCK(mTyp) ((mTyp >= Mtyp_TESS)&&(mTyp < Mtyp_BMP))

// MDL_MTYP_IS_IMG_MOCK    check if model is image or mockup
//   (tess obj, stl, wrl, bmp, jpg)
#define MDL_MTYP_IS_IMG_MOCK(mTyp) (mTyp >= Mtyp_TESS)


// modelname .tess-file  <tmpDir>/Mod_<model>.tess
#define MDL_fnModTess_mNam(fNam,mdlNam)\
 sprintf(fNam,"%sMod_%s.tess",AP_get_tmp_dir(),mdlNam)

// EOF
