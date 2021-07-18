# Start Debug-Session mit ./go debug

# damit wird nach Laden einer DLL gestoppt; nun kann man man manuell
# ein break fuer eine Funktion in der DLL setzen.
#break OS_debug_dll_

######### define Functions to stop
break AP_debug__
break TX_Error
#break UTO_dump__
#break UTO_dump_s_
#break UTO_dump_obj
#break UT3D_stru_dump

#break MSH_aload_pTab
#break "xa_ui_gr.c":2760
#condition 2 sInd == 80
#condition 9 ED_work_CAD::lNr == 22
#condition 9 *dllNam == "xa_stp_r.so"

#watch wcg_geoNam
#watch 0x83d6d40
#watch pln_tab[1].p

# damit wird nach dem Laden einer DLL gestoppt.
#break dlsym

#directory xa/
#set solib-search-path "xa/"
#sharedlibrary libxa_test.so
#add-symbol-file libxa_test.so.1

# zum run hier fuer debug die Startparameter zufuegen; else nur run
#run comp Data/unknown.gcad tst
#run comp STP_U/4bar.gcad tst
#run tst
#run
#c

# Stop beim load einer DLL;
# verursacht dzt einen Stop; somit kann man break fuer sharedLib eingeben.
#catch load
#catch load libxa_test.so.1
#set stop-on-solib-events 1

#run Data_U/sample_dimen1.gcad
#run comp Data/sample_DiagEl1.gcad
#run mode_cad_3_0
#run Data/sample_Iges2.igs
#run comp Data_U/unknown.gcad
#run comp Data_U/test_cv_rev1.gcad
#run comp Data_U/sample_mod_nut1.gcad
#run comp Data/test_all_curvtypes.gcad
#run comp STP_U/Err1.gcad
#run Data/sample_area_bsp0.gcad
#run Data_U/test_all_curvtypes.gcad
#run comp Data/test_models_6.gcad
#run Data_U/unknown.gcad
#run Data_U/test_sur_cyl_5.gcad mode_cad_7_8
#run Data_U/Renault1.gcad mode_cad_10_1
#run ELE1/unknown.gcad mode_cad
#run comp Data_U/test_mod_1.gcad mode_cad
#run comp Data_U/t1.gcad mode_cad
#run comp mode_cad

#break GUI_popup__
#break GL_set_ocv
#break UTF_insert1
#break GL_Init_col
#break MSH2D_view_i2ptb2
#cond 3 Ind == 0
#break UI_butCB
#break DB_mdlNam_iBas

#watch GR_TAB_IND
#watch ((Circ*)TSU_ox2.data)->vz.dy
#
