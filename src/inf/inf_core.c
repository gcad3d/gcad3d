/* ../inf/inf_core.c




================================================================== */
void INF_CORE__ (){        /*! \code
INF_CORE__     control for all modules

INF_PLU_COR__ core-plugin-control                   ??
INF_PLU_USR__ user-plugin-control                   PLU_Loa ../xa/xa_plu.c
INF_IMP_EXP__ import-export-modules (core-plugins)          ../exp/xx.c
INF_CTRL__    remote-control (core-plugin)                  ../xa/xa_ctrl.c
INF_PRG__     script-control (core-plugin)                  ../xa/xa_prg.c
INF_PRC__     process-control (core-plugin)                 ../xa/xa_proc.c
INF_MSGWIN__  messagewindow (at bottom)


================================================================== \endcode */}
void INF_PLU_COR__ (){        /*! \code

INF_PLU_COR__ core-plugin

list makefiles of core-plugins:        ls xa_*.mak

- can be unloaded at return or not
  if plugin uses selections must kept alive; 
  unload it with GUI_idle__ (OS_dll_unload_idle, "<pluginName>");





Example: pluginName = PcoreTest

  // call plugin from core:
  int typ; long dbi; void *pa[2];
  // optional: for rebuild must be unloaded
  OS_dll_do ("xa_PcoreTest", NULL, NULL, 2);
  // optional: reBuild dll
  irc = OS_dll_build ("xa_PcoreTest.so");
  if(irc != 0) return -1;
  // load parameterBlock pa
  typ = Typ_PT; dbi = 24L;
  pa[0] = &typ; pa[1] = &dbi;
  // load, start, do not unload core-plugin
  OS_dll_do ("xa_PcoreTest", "PcoreTest__", &pa, 1);


File ../xa/PcoreTest.c:
  // export the mainEntry for MS
  #ifdef _MSC_VER
  __declspec(dllexport) int EDMPT__ (void *pa[]);
  #define extern __declspec(dllimport)
  #endif
  //
  int PcoreTest__ (void *pa[]) {
    printf("PcoreTest__ typ=%d dbi=%ld\n",*((int*)pa[0]), *((long*)pa[1]));
    ..
    // unload core-plugin after all operations closed:
    GUI_idle__ (OS_dll_unload_idle, "xa_PcoreTest");
    ..
    return 0; // 0-OK,close-dll; 1=OK,keep-dll; -1=error,close-dll
  }

  
File xa_PcoreTest.mak:
DLLNAM = xa_PcoreTest
SRC1 = ../xa/PcoreTest.c
DIRSRC1 := ../xa/
include gcad_dll.mak



================================================================== \endcode */}
void INF_PLU_USR__ (){        /*! \code

INF_PLU_USR__ user-plugin-control                   PLU_Loa ../xa/xa_plu.c

- create file <fnamBuild>.mak    see Demo*.mak, eg DemoPlugin_Resolv.mak
- create file <fnamPrg>.c        see ../APP/DemoPlugin_Resolv.c
  - must have mainentry "int gCad_main ()"


Doc:
../../doc/html/Plugin_en.htm                        <<<< update- include HERE
../../doc/gcad_doxygen/Userinteractions.dox
../../doc/gcad_doxygen/Objects-Create.dox
../../doc/gcad_doxygen/Objects-Resolve.dox
../../doc/gcad_doxygen/Attributes.dox


DLL_run1         connect oder run oder unload DLL.  Always gCad_main
DLL_run2         build & connect & run & unload DLL.



================================================================== \endcode */}
void INF_IMP_EXP__ (){        /*! \code

INF_IMP_EXP__ import-export-modules (core-plugins)          ../exp/xx.c



================================================================== \endcode */}
void INF_CTRL__ (){        /*! \code

INF_CTRL__    remote-control (core-plugin)                  ../xa/xa_ctrl.c


Doc:
../..doc/html/RemoteControl_en.htm



================================================================== \endcode */}
void INF_PRG__ (){        /*! \code

INF_PRG__     script-control (core-plugin)

Src:
../xa/xa_prg.c

Doc:
../../doc/html/Appli_en.htm



================================================================== \endcode */}
void INF_PRC__ (){        /*! \code

INF_PRC__     process-control (core-plugin)

Src:
../xa/xa_proc.c
PRC*.mak

Doc:
../../doc/html/Process_en.htm


================================================================== \endcode */}
void INF_MSGWIN__ (){        /*! \code

INF_MSGWIN__  messagewindow (at bottom)

../ut/ut_ui_TX.c

UI_winTX_rmLast             remove last message (for overwrite)


TX_Print
  TX_Write
    UI_winTX_prt
      GUI_msgwin_prt




================================================================== \endcode */}
// eof

