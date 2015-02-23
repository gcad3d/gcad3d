// ../gui/gui_types.h


// was ../ut/ut_geo.h
/// events
#define TYP_EventEnter      400  ///< event enter
#define TYP_EventLeave      401  ///< event leave
#define TYP_EventPress      402  ///< event press (key)
#define TYP_EventRelease    403  ///< event release (key)
#define TYP_EventMove       404  ///< event move (mouse)
#define TYP_EventDraw       405  ///< event draw
#define TYP_EventConfig     406  ///< event config 
#define TYP_EventUnmap      407  ///< event unmap (widget disappears)
#define TYP_EventExit       409  ///< event exit (widget destroyed)

/// mouseEvents

// ../ut/ut_geo.h
/// devices
#define TYP_DeviceKeyb      410  ///< device keyboard
#define TYP_DeviceMouse     411  ///< mousebutton / scroll-wheel
// #define TYP_DeviceButton    312  ///< device screen
// #define TYP_DeviceScreen   311  ///< device screen

/// widgets
#define TYP_GUI_Unknown     420
#define TYP_GUI_Win         421
#define TYP_GUI_BoxH        422
#define TYP_GUI_BoxV        423
#define TYP_GUI_Label       424
#define TYP_GUI_Button      425
#define TYP_GUI_CheckBox    426
#define TYP_GUI_Entry       427
#define TYP_GUI_Slider      428
#define TYP_GUI_MenuShell   429
#define TYP_GUI_MenuItem    430
#define TYP_GUI_OptMen      431      /// option-menu
#define TYP_GUI_MsgWin      432      /// message-window
#define TYP_GUI_List        433      /// selection-list; 1 or 2 columns
#define TYP_GUI_Frame       434      /// 
#define TYP_GUI_Toolbar     435      /// 
#define TYP_GUI_Notebook    436      /// 
#define TYP_GUI_Tree        437
#define TYP_GUI_BoxGL       438
#define TYP_GUI_Editor      439


//================================================================
 
/// get data of callback
#define GUI_DATA_EVENT      *(int*)data[0]

#define GUI_DATA_I1         *(int*)data[1]
#define GUI_DATA_L1         *(long*)data[1]
#define GUI_DATA_D1         *(double*)data[1]
#define GUI_DATA_S1         (char*)data[1]
#define GUI_DATA_PTR_I1     (int*)(data[1])

#define GUI_DATA_I2         *(int*)data[2]
#define GUI_DATA_L2         *(long*)data[2]
#define GUI_DATA_S2         (char*)data[2]

#define GUI_DATA_I3         *(int*)data[3]
#define GUI_DATA_S3         (char*)data[3]

#define GUI_DATA_I4         *(int*)data[4]
#define GUI_DATA_S4         (char*)data[4]

#define GUI_DATA_I5         *(int*)data[5]
#define GUI_DATA_S5         (char*)data[5]



/// values mouse - TYP_DeviceMouse
#define GUI_MouseL          1  ///< left mousebutton
#define GUI_MouseM          2  ///< mid mousebutton
#define GUI_MouseR          3  ///< right mousebutton
#define GUI_Mouse2L         4  ///< doubleclick
#define GUI_MouseScUp       5  ///< mouse scroll up
#define GUI_MouseScDown     6  ///< mouse scroll down

/// values keyboard - TYP_DeviceKeyb
/// /usr/include/gtk-2.0/gdk/gdkkeysyms.h
/// see also /usr/include/X11/keysymdef.h
#define GUI_KeyEsc          0xff1b
#define GUI_KeyTab          0xff09
#define GUI_KeyReturn       0xff0d           // 65293
#define GUI_KeyBackSpace    0xff08      // "remove-previous-char"
#define GUI_KeyDel          0xffff      // Delete, "remove-next-char"
#define GUI_KeyNumDel       0xff9f      // Delete in numeric-key-block

#define GUI_KeyPageUp       0xff55           // 65365 ff9a 65434
#define GUI_KeyPageDown     0xff56           // 65366 ff9b 65435

#define GUI_KeyCurUp        0xff52           // 
#define GUI_KeyCurDown      0xff54           // 65364
#define GUI_KeyCurLeft      0xff51
#define GUI_KeyCurRight     0xff53

#define GUI_KeyShift_L      0xffe1           // 65505
#define GUI_KeyShift_R      0xffe2
#define GUI_KeyControl_L    0xffe3           // 65507
#define GUI_KeyControl_R    0xffe4           // 65508
#define GUI_KeyAlt_L        0xffe9           // 65513
#define GUI_KeyAlt_R        0xffea           // 65514
#define GUI_KeyMeta_L       0xffe7           // GTK: Shift + Alt !
#define GUI_KeyMeta_R       0xffe8
                                             // ffeb=65515=windows-key
#define GUI_KeyF1           0xff91           // XK_F1 XK_KP_F1 
#define GUI_KeyF2           0xff92
#define GUI_KeyF3           65472             // 0xff93
#define GUI_KeyF4           65473
#define GUI_KeyF5           0xffc2

// MouseButtons: mouseButtonNr(1|2|3);
// DoubleClick(GDK_2BUTTON_PRESS)=4;
// ScrollUp(GDK_SCROLL_UP)=5; ScrollDown(GDK_SCROLL_DOWN)=6;
// ReturnKey=13;

#define GUI_Modif_shift    1              // 2=SHIFT LOCK
#define GUI_Modif_ctrl     4
#define GUI_Modif_alt      8
/// if(modVal & GUI_Modif_alt)   printf(" ALT IS ON\n");


#define GUI_STYL_normal    0
#define GUI_STYL_activ     1
#define GUI_STYL_passiv    2



//----------------------------------------------------------------
#define UI_FuncInit       90
#define UI_FuncInit1     191
#define UI_FuncInit2     192
#define UI_FuncInit3     193
#define UI_FuncClear     194
#define UI_FuncGet        91
#define UI_FuncSet        92
#define UI_FuncDisable    93
#define UI_FuncEnable     94
#define UI_FuncFocus      95
#define UI_FuncPrev       96
#define UI_FuncNext       97
#define UI_FuncWork       98
#define UI_FuncKill       99
#define UI_FuncOK        100
#define UI_FuncCancel    101
#define UI_FuncExit      102
#define UI_FuncHelp      103
#define UI_FuncNO        104

#define UI_FuncInsert    110
#define UI_FuncModify    111   
#define UI_FuncDelete    112
#define UI_FuncUndo      113
     
#define UI_FuncUCB      1000         // Usercallbacks; 1000-1999
#define UI_FuncUCB1     1001
#define UI_FuncUCB2     1002
#define UI_FuncUCB3     1003
#define UI_FuncUCB4     1004
#define UI_FuncUCB5     1005
#define UI_FuncUCB6     1006
#define UI_FuncUCB7     1007
#define UI_FuncUCB8     1008
#define UI_FuncUCB9     1009
#define UI_FuncUCB10    1010
#define UI_FuncUCB11    1011
#define UI_FuncUCB12    1012
#define UI_FuncUCB13    1013
    

// EOF
