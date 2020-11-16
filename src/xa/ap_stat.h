// AP_STAT ap_stat - application-status


// sysStat    1=init GUI OK; 2=init OpenGL OK; 3=init model OK.
// errStat    0=Ok, no Error
// errLn      SourceLineNr where Error occured
// batch      0=interaktiv, 1=Batchmode.
// texture    0=no, hardware does not accept textures; 1=Yes, Ok.
// build      Compile,Linker not checked/not available; 1=available.
// comp       Compile plugins off; 1=on.
// APP_stat   0=mainActive; 1=<APP_act_nam> (plugin) active.
// TUT_stat   ScreeCast on=1  off=0
// subtyp     0=VRML1, 1=VRML2
// jntStat    0=unInitilized, 1=open  (DBFile <tmp>/joints)
// debStat    0=normal (debug off); 1=debug_ON
// tstDllStat 0=normal (OFF); 1=testdll_ON
// mdl_modified   UNUSED
// mdl_box_valid  AP_mdlbox_invalid_ck - 0=valid, 1=void,invalid
// mdl_stat   see MDLSTAT_empty ..
// brw_stat   see BRWSTAT_active ..
// start_rcmd 0=no; 1=start-remoteCommandFile
// err_hide   0=normal-display errors;   1=hide-errors
// iActStat   0=normal, 1=Interactivity_ON
// cadIniM    CAD - init - mainFunction
// cadIniS    CAD - init - subFunction

typedef struct {int errLn; short sysStat, errStat;
                char      cadIniM, cadIniS, mdl_stat, brw_stat,
                          subtyp, cuu1, cuu2, cuu3;
                unsigned  batch:1,
                          texture:1,
                          build:1,
                          APP_stat:1,
                          TUT_stat:1,      // 5
                          jntStat:1,
                          debStat:1,
                          iActStat:1,
                          tstDllStat:1,
                          mdl_modified:1,  // 10
                          mdl_box_valid:1,
                          comp:1,
                          start_rcmd:1,
                          err_hide:1,
                          uuBits:18; }                         AP_STAT;



// eof
