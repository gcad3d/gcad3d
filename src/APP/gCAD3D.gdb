# Start Debug-Session mit ./go d
#watch wcg_geoNam
#break GL_fix_DL_ind
#watch 0x83d6d40
#watch pln_tab[1].p
#watch ci_tab
#watch WC_modnam
#break ED_work_CurSet

#break tst1.o:IGE_r_120
#run comp debug tst1
#run comp
#break UME_realloc
#break IGE_rw_102
#break DB_allocCDAT
#run igi dat/M2_1.igs

# Break am Beginn des IGES-Import:
#break UT3D_parsru_pt
#break IGE_r_work__

# funktioniert leider nicht:
#break free


# damit wird nach Laden einer DLL gestoppt; nun kann man man manuell
# ein break fuer eine Funktion in der DLL setzen.
#break OS_debug_dll_

# damit wird im Hauptmodul gestoppt
break AP_debug__

break TX_Error
#break UTO_dump__
#break UTO_dump_s_
#break UTO_dump_obj
#break UT3D_stru_dump
#break DB_StoreModBas

#break GR_Disp_box
#break GR_Disp_pt
#break GR_Disp_ac
#break GR_Disp_cv
#break GR_Disp_pTab
#break GR_Disp_txi



# aktuelle Function:
#break UT3D_parsru_pt:668
#break UI_undo_do
#break ../ut/ut_sbsp.c:1168
#break CTLG_mnam_modelnam
#break DB_StoreModBas
#condition 6 mode == -2
#break CTLG_mnam_modelnam
#condition 12 ptMax==4148



# damit wird nach dem Laden einer DLL gestoppt.
#break dlsym

#directory xa/
#set solib-search-path "xa/"
#sharedlibrary libxa_test.so

#add-symbol-file libxa_test.so.1


######### define hier Funktion to stop
# vor dem Ablauf stoppen; hier ein watch setzen
#break MSH_aload_pTab
#break NC_setRefsys
#break AP_SMD_prv
#break "xa_ui_gr.c":2760
#condition 9 ED_work_CAD::lNr == 22

#break bspl_pol_bsp
#break ED_work_END
#break UTRA_def__
#break DB_QueryDef
#condition 5 typ == 9
#break STP_r_cre2
#condition 2 sInd == 80

#condition 9 *dllNam == "xa_stp_r.so"
#break STP_r_creMod_Run

# zum run hier fuer debug die Startparameter zufuegen; else nur run
#run tst
#run
#c


# Stop beim load einer DLL;
# verursacht dzt einen Stop; somit kann man break fuer sharedLib eingeben.
#catch load
#catch load libxa_test.so.1
#set stop-on-solib-events 1

#
