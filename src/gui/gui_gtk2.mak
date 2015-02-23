# get SRCGUI (filList), GUICP (compile with gtk2), GUILIB (link with gtk2) 
# OS = Linux32|Linux64
# VGUI = gtk2


SRCGUI =\
 ../gui/gui_base.c\
 ../gui_gtk2/gtk_base.c\
 ../gui_gtk2/gtk_label.c\
 ../gui_gtk2/gtk_button.c\
 ../gui_gtk2/gtk_menu.c\
 ../gui_gtk2/gtk_entry.c\
 ../gui_gtk2/gtk_image.c\
 ../gui_gtk2/gtk_dlg_files.c\
 ../gui_gtk2/gtk_dlg_std.c\
 ../gui_gtk2/gtk_msgWin.c\
 ../gui_gtk2/gtk_lists.c\
 ../gui_gtk2/gtk_multichoice.c\
 ../gui_gtk2/gtk_container.c\
 ../gui_gtk2/gtk_tree_it.c\
 ../gui_gtk2/gtk_ed1.c\
 ../gui_gtk2/gtk_opengl.c\
 ../gui_gtk2/gtk_gl_x11.c



GUICP = `pkg-config --cflags gtk+-2.0` -Dgtk2
GUILIB = `pkg-config --libs gtk+-2.0`


# GTK for MinGW
#ifeq "$(OS)" "MS32"
#
#  gtk_inc_dir = ../gtk_MS_216/include
#  GUICP = -mms-bitfields -DGTK2 -I$(gtk_inc_dir)/gtk-2.0 -I$(gtk_inc_dir)/glib-2.0 -I$(gtk_inc_dir)/cairo -I$(gtk_inc_dir)/pango-1.0 -I$(gtk_inc_dir)/atk-1.0 -D_MSC_VER
#
## gtk_lib_dir = /x/Devel/dev/gCAD3D/gtk_MS_216/lib
## GUILIB = -L$(gtk_lib_dir) gtk-win32-2.0.lib gdk-win32-2.0.lib gdk_pixbuf-2.0.lib glib-2.0.lib gobject-2.0.lib
#
#  gtk_lib_dir = ../gtk_MS_216/lib
#  GUILIB = $(gtk_lib_dir)/gtk-win32-2.0.lib $(gtk_lib_dir)/gdk-win32-2.0.lib $(gtk_lib_dir)/gdk_pixbuf-2.0.lib $(gtk_lib_dir)/glib-2.0.lib $(gtk_lib_dir)/gobject-2.0.lib
#
#endif


# EOF
