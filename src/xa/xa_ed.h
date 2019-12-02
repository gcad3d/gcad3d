// xa_ed.h


// AP_mode_step        defaultmode; CAD-input, 
//   ED_step() ED_work_ENTER()
#define  AP_mode_step  0                    /// see AP_mode__

// AP_mode_go
//   update model (SRCU_mod__) ED_go()
#define  AP_mode_go    1

// AP_mode_enter       CR from keyBd (CAD)
//   skip TPC_JUMP TPC_MAC TPC_GEO TPC_END
#define  AP_mode_enter 3

// AP_mode_END         automatic-run; no display of symbols;
//   used by END-button  ED_skip_start(resolv subModels)
#define  AP_mode_END   4


  int   ED_Init          ();
  // int   ED_Init_All      ();
  int   ED_Reset         ();
  int   ED_Read_Next     (char*);
  int   ED_Read_Line     (char* buf);
  char* ED_Read_cPos     ();
  // int  ED_GetActLin     (int*, char*);
  int   ED_GetNxtLin     (int*, char*);
  int   ED_GetLinNr      (char*, int);
  // int   ED_Get_LineNr    ();
  int   ED_get_lnr_act   ();
  int   ED_get_mac_fil   ();
  int   ED_work_file     (char*);
  int   ED_cont_file     ();
  int   ED_step          ();
  int   ED_go            ();
  int   ED_enter         ();
  int   ED_skip_start    ();
  int   ED_skip_end      ();
  int   ED_update        ();
  void  ED_set_delay     (int);

  int   ED_query_CmdMode ();
  int   ED_query_fsiz    (long*, long*);

  char* ED_get_actPos   ();

  int   ED_add_Def       (char *buf);
  int   ED_add_Line      (char *buf);
  int   ED_add_Text      (int, long, char*);
  int   ED_add_objSel    (int, long, char*);
  int   ED_add_objNam    (int, int*, long*, char*);
  int   ED_add_objInd    (int);
  int   ED_add_objTxt    (char*);
  int   ED_add_File      (char*, int);
  int   ED_new_File      (char*);
  int   ED_Add           (char*);

  int   ED_query_CmdMode ();

  // void ED_step_execute  ();
  // void ED_line_execute  (char*);

  void ED_line_work     (char*);
  void ED_lineNr_work   (int);
  void ED_line_enter    ();

  void ED_startKon2     (int);

  void ED_delete        ();
  void ED_del_deakt     ();
  void ED_del_reakt     ();
  void ED_del_von       ();
  void ED_del_bis       ();
  void ED_del_on        ();
  void ED_del_off       ();

  int  ED_work_GO       ();
  int  ED_work_STEP     ();
  int  ED_work_END      ();
  void ED_work_ENTER    ();
  int  ED_work_CurSet   (int bis);
  int  ED_Run           ();
  // void ED_work_PP       ();
  // void ED_PP_run        (char* ppnam);

  //void ED_hili_tab     (int, int*, long*);

/*=============== EOF ======================*/
