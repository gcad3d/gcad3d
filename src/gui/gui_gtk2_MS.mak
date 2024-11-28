# CPGUI LIBGUI SRCGUI OBJGUI
# using DIR_EXE

$(info - entering ../gui/gui_gtk2_MS.mak)


SRCGUI=\
 ../gui/gui_base.c\
 ../gui_gtk2_MS/gtk_base.c\
 ../gui_gtk2_MS/gtk_button.c\
 ../gui_gtk2_MS/gtk_lists.c\
 ../gui_gtk2_MS/gtk_ed1.c\
 ../gui_gtk2_MS/gtk_entry.c\
 ../gui_gtk2_MS/gtk_menu.c\
 ../gui_gtk2_MS/gtk_label.c\
 ../gui_gtk2_MS/gtk_image.c\
 ../gui_gtk2_MS/gtk_dlg_std.c\
 ../gui_gtk2_MS/gtk_container.c\
 ../gui_gtk2_MS/gtk_multichoice.c\
 ../gui_gtk2_MS/gtk_msgWin.c\
 ../gui_gtk2_MS/gtk_tree_it.c\
 ../gui_gtk2_MS/gtk_opengl.c


#..\gui_gtk2_MS\gtk_dlg_files.c\
#..\gui_gtk2_MS\gtk_printer.c\

#-------------------------------------------
OBJGUI = $(SRCGUI:.c=.obj)
#OBJGUI = $(OBJGUI:/=\)



#-------------------------------------------
GTK_BAS_DIR=$(DIR_EXE)

CPGUI = -Dgtk2_MS\
 `pkg-config gtk+-2.0 --cflags --libs`

# -I /c/msys64/mingw64/include/gtkgl-2.0\


LKGUI = -L/c/msys64/mingw64/lib


#-------------------------------------------
GTK_LIB_DIR=$(GTK_BAS_DIR)\lib

#LIBGUI=/c/msys64/mingw64/lib
#LIBGUI = -lgtkgl-2.0


$(info - OBJGUI = $(OBJGUI))
$(info - CPGUI = $(CPGUI))
$(info - LKGUI = $(LKGUI))
$(info - LIBGUI = $(LIBGUI))
$(info - exit ../gui/gui_gtk2_MS.nmak)

# eof
