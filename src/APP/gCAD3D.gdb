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


#break CTLG_mnam_modelnam
#cond 3 Ind == 0
#run Data_U/sample_dimen1.gcad
#run comp Data/sample_DiagEl1.gcad
#run
#run Data/samp_wrl1.wrl
run Data/CarPort1.gcad
#run
#watch ((Circ*)TSU_ox2.data)->vz.dy
#break sele_set__
break UI_popSel_CB_prev
c
#
