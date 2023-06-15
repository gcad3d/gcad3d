# ../gui/gui_gtk2.mak
# get SRCGUI (filList), GUICP (compile with gtk2), GUILIB (link with gtk2) 
# OS = Linux32|Linux64
# VGUI = gtk2

$(info - entering ../gui/gui_src_MSYS.mak)

SRCGUI=\
 ../gui_gtk2/gtkgl_bnd.c\
 ../gui_gtk2/gtk_base.c\
 ../gui_gtk2/gtk_label.c\
 ../gui_gtk2/gtk_button.c\
 ../gui_gtk2/gtk_menu.c\
 ../gui_gtk2/gtk_entry.c\
 ../gui_gtk2/gtk_image.c\
 ../gui_gtk2/gtk_dlg_std.c\
 ../gui_gtk2/gtk_msgWin.c\
 ../gui_gtk2/gtk_lists.c\
 ../gui_gtk2/gtk_multichoice.c\
 ../gui_gtk2/gtk_container.c\
 ../gui_gtk2/gtk_tree_it.c\
 ../gui_gtk2/gtk_ed1.c\
 ../gui_gtk2/gtk_opengl.c\
 ../gui_gtk2/gtk_base.c



#../gui_gtk2/gtk_gl_MS.c
#../gui_gtk2/GL_C-KIT.c
#../gui_gtk2/gtk_bind_gl.c


#../gui/gui_base.c\    - in SRCG/OBJG
#../gui_gtk2/gtk_gl_x11.c
#../gui_gtk2/gtk_bind_gl.c
#../gui_gtk2/gtk_dlg_files.c\
#../gui_gtk2/gtk_printer.c\


$(info - SRCGUI = $(SRCGUI))

# EOF
