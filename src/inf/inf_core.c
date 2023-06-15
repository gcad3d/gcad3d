/* ../inf/inf_core.c




================================================================== */
void INF_CORE__ (){        /*! \code
INF_CORE__     control for all modules

INF_DLL_KEX   cad-kernel-extension-dll                 
INF_DLL_USR   user-plugin-control                   PLU_Loa ../xa/xa_plu.c
INF_IMP_EXP__ import-export-modules (core-plugins)          ../exp/xx.c
INF_CTRL__    remote-control (core-plugin)                  ../xa/xa_ctrl.c
INF_PRG__     script-control (core-plugin)                  ../xa/xa_prg.c
INF_PRC__     process-control (core-plugin)                 ../xa/xa_proc.c
INF_MSGWIN__  messagewindow (at bottom)
INF_Grp__     Group                                         ../xa/xa_grp.c
INF_Ico__     Icons                                         ../xa/xa_ico.c     Ico
INF_AP_STAT   program-status-bits

INF_symDir    symbolic-directory modelfilename

================================================================== \endcode */}
void INF_DLL_KEX (){        /*! \code

INF_DLL_KEX cad-kernel-extension-dll

list makefiles of core-plugins:        ls xa_*.mak
- dll's are located in <gcad_dir_bin>

- can be unloaded at return or not
  if plugin uses user-interactions (selection, keyIn ..) must be kept alive; 


Kernel-extension-dll's:
  prc_dll    (NC.-)process
  dll_imp    model-import       
  dll_exp    modelexport
  dll_pri    dll for print pdf


- see AP_kex_exec ()




================================================================== \endcode */}
void INF_DLL_USR (){        /*! \code

INF_DLL_USR user-plugin-control 

- dll's are located in <gcad_dir_bin>plugins/

- in directory <base>gcad3d/src/UIX/
- create file <fnamBuild>.mak           see Demo*.mak, eg DemoPlugin_Resolv.mak
- create file ../APP/<fnamPrg>.c        see ../APP/DemoPlugin_Resolv.c
  - must have mainentry "int gCad_main ()" and "int gCad_fini"

First time build plugin with:
. ./devbase.sh && make -f <fnamBuild>.mak

All following builds can be done while gcad is running -
   - activate Options/compile DLL
   - Ctrl-p rebuilds last used user-plugin


Functions for plugins:
gCad_main                  main entry
AP_UserKeyIn_get           attach keypress to plugin
AP_UserSelection_get       attach selection to plugin
AP_UserMousemove_get       attach mousemove to plugin
AP_User_reset              reset keypress, selection, mousemove
gCad_fini                  exit plugin


Plugin-dll:
  ptr_PLU   ../xa/xa_dll.c
AP_plu_start               get plugin-name from user; start plugin ..
  AP_plu_exec
    DLL_plu__              rebuild/load/connect-gCad_main
      .. - in dll -
      gCad_fini - in dll -
        AP_User_reset - in core -     reset to inactive;



UI_Set_typPrg                           // disp type active program-name
UI_Set_actPrg (APP_act_nam, <col>);     // disp active program-name

ptr_PLU
APP_act_typ=3
APP_act_nam=dllNam
AP_stat.APP_stat     1=active;


Doc:
../../doc/html/Plugin_en.htm                        <<<< update- include HERE
../../doc/gcad_doxygen/Userinteractions.dox
../../doc/gcad_doxygen/Objects-Create.dox
../../doc/gcad_doxygen/Objects-Resolve.dox
../../doc/gcad_doxygen/Attributes.dox




================================================================== \endcode */}
void INF_IMP__ (){        /*! \code

- import cad-models;

using AP_kex_exec

================================================================== \endcode */}
void INF_EXP__ (){        /*! \code

- export cad-models;
                       IMPORT         EXPORT
#define Mtyp_DXF        OK            OK  subModels ??
#define Mtyp_Iges       OK            BUG export subModels !!!
#define Mtyp_Step       OK            test_mod_1.gcad OK, ELE1/box1.gcad nicht
#define Mtyp_3DS        OK            NYI
#define Mtyp_LWO        OK            NYI
#define Mtyp_XML        <<< TODO      NYI
#define Mtyp_SVG        <<< no imp    tw

#define Mtyp_TESS       OK            tw
#define Mtyp_WRL        OK            tw - no subModels
#define Mtyp_WRL2       <<<           tw (no PRISM)
#define Mtyp_OBJ        OK            tw - no subModels
#define Mtyp_STL        OK            tw - no subModels


using AP_kex_exec


================================================================== \endcode */}
void INF_CTRL__ (){        /*! \code

INF_CTRL__    remote-control (core-plugin)                  ../xa/xa_ctrl.c


APP_act_typ=4; RPC

RPC_Loa ()
RPC_restart ()


Doc:
../../doc/html/RemoteControl_en.htm



================================================================== \endcode */}
void INF_PRG__ (){        /*! \code

INF_PRG__     Applications (*.gcap) = script-control; 

- APP_act_typ=1; no DLL used

source-directory for *.gcap
../../gCAD3D/prg/

Src:
../xa/xa_prg.c

Doc:
../../doc/html/Appli_en.htm


- writes codes into file <tmp>/PRG_ACT.gcad


Functions:
PRG_Loa
PRG_win__



================================================================== \endcode */}
void INF_PRC__ (){        /*! \code

INF_PRC__     process-control (core-plugin)

APP_act_typ=2; eg Data/sample_PRC_cut1_1.gcad

Src:
../xa/xa_proc.c
PRC*.mak

Doc:
../../doc/html/Process_en.htm


PRC_activate__
  GUI_radiobutt_set UI_butCB |MAN| .. APT_work_AppCodTab
    PRC_init
      DLL_dyn__ DLL_LOAD_only, dllNam
      DLL_dyn__ DLL_CONNECT, "PRCE__"
      PRC__ (-1, "INIT__");
        PRCE_func__ |INIT__|
          PRCE_tb_init                 // activate toolbar            TEST-RAUS <<<<
            PRCE_tb_win
              PRCE_lst_postprocs

Variables:
APP_act_typ = 2;                        // "PRC "
APP_act_nam             // name of process (dll, eg "process_procDemo1")
APP_act_proc            // name of processor (dllNam)
AP_stat.APP_stat = 1;   // active
NC_procNr               // 1 = active; 0=not; use AP_stat.APP_stat ?





================================================================== \endcode */}
void INF_MSGWIN__ (){        /*! \code

INF_MSGWIN__     messagewindow (at bottom)
INF_MSG_new      functions for system-messages (by keyword, with translations)



../ut/ut_ui_TX.c

UI_winTX_rmLast             remove last message (for overwrite)


TX_Print                  ../ut/ut_ui_TX.c   - 
  TX_Write
    UI_winTX_prt
      GUI_msgwin_prt

TX_Print - replacements for eg ../ut/os_uix.c:         ../ut/ut_TX.c
or:
void TX_Print (char* txt, ...) { printf("%s\n",txt); }



================================================================== \endcode */}
void INF_Grp__ (){        /*! \code

DL_Att* GR_ObjTab[].grp_1 = groupBit in displList; 0=belongs to active Group, 1=not

ObjDB*  GrpTab[]   keeps typ, dbi, dli, stat


Group-bit in dispList (GR_ObjTab[ind].grp) -
- DL_grp1__() does:
  - set group-bit GR_ObjTab[ind].grp in dispList and hilite obj 


ObjDB*  GrpTab[] -
- Grp_add__() does:
  - add obj to GrpTab[] and display GrpNr in Label UID_ouf_grpNr
  - do not set group-bit in dispList and not hilite obj



Files:
../xa/xa_grp.c
../gr/ut_DL.c       DL_grp1_*


Functions:
Grp_*               ../xa/xa_grp.c
Grp_get_ts    Modifying group sets TimeStamp, get it with Grp_get_ts.


================================================================== \endcode */}
void INF_Ico__ (){        /*! \code

INF_Ico__     Icons  Ico


AP_get_ico_dir returns os_ico_dir = <gcad_dir_bin>icons/
/usr/share/gcad3d/icons/
DevDir:   ../../icons/*.png

../xa/xa_ico.c           // enum ICO_<nam>
../xa/xa_ico.h

Icons used in Browser:
- create <icoNam>.png
Ico_init                  // load icons 
  GUI_Ico_init              // load a png-file



================================================================== \endcode */}
void INF_symDir (){        /*! \code

A symbolic file name consists of "{symbolic-directory}/{extraPath}{filename}" 
Example of symbolic file name: "Data/Niet1.dat" 
Symbolic-directory can have size up to <SIZMFSym> chars.

All symbolic-directories are defined in file <cfgdir>/dir.lst
  Format:
symbol path
Example:
DXF_U   /mnt/serv2/devel/cadfiles/dxf/
DXF_W  X:\Devel\cadfiles\dxf\

Files:
../../gCAD3D/cfg_Linux/dir.lst


Functions:
MDLFN_fDir_syDir           get resolved symbolic filename

Files:
../xa/mdlfn.c

see:  INF_stru_FN
stru_FN    {char symDir[128], fDir[128], fNam[128], fTyp[40], iTyp;}



Sizes of filenamefields: see ../xa/mdl__.h
  SIZMFTot   256   // symbolic|safe-modelnames (symDir/fNam.fTyp), fDir
  SIZFNam    400   // full filename


================================================================== \endcode */}
void INF_stru_FN (){        /*! \code

filename-object (symbolic-directory, full-directory, filename, filetyp ..)

stru_FN    {char symDir[128], fDir[128], fNam[128], fTyp[40], iTyp;}
   .symDir AP_mod_sym   Data/symEl1            symbolic-directory + extrapath
   .fDir   AP_mod_dir   ~/gCAD3D/dat/symEl1/   full-directory + extrapath
   .fNam   AP_mod_fnam  res1                   filename
   .fTyp   AP_mod_ftyp  gcad                   filetyp



================================================================== \endcode */}
void INF_AP_STAT (){        /*! \code

program-status-bits

AP_STAT  in ../xa/ap_stat.h






================================================================== \endcode */}
// eof

