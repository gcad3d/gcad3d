# gui_gtk2.mak - set CPGUI LKGUI

$(info - entering ../gui/gui_gtk2.mak)

CPGUI = `pkg-config --cflags gtk+-2.0` -Dgtk2
LKGUI = `pkg-config --libs gtk+-2.0`


#  gtk_inc_dir = ../gtk_MS_216/include
#  GUICP = -mms-bitfields -DGTK2 -I$(gtk_inc_dir)/gtk-2.0 -I$(gtk_inc_dir)/glib-2.0 -I$(gtk_inc_dir)/cairo -I$(gtk_inc_dir)/pango-1.0 -I$(gtk_inc_dir)/atk-1.0 -D_MSC_VER
#
## gtk_lib_dir = /x/Devel/dev/gCAD3D/gtk_MS_216/lib
## GUILIB = -L$(gtk_lib_dir) gtk-win32-2.0.lib gdk-win32-2.0.lib gdk_pixbuf-2.0.lib glib-2.0.lib gobject-2.0.lib
#
#  gtk_lib_dir = ../gtk_MS_216/lib
#  GUILIB = $(gtk_lib_dir)/gtk-win32-2.0.lib $(gtk_lib_dir)/gdk-win32-2.0.lib $(gtk_lib_dir)/gdk_pixbuf-2.0.lib $(gtk_lib_dir)/glib-2.0.lib $(gtk_lib_dir)/gobject-2.0.lib
#


$(info - exit ../gui/gui_gtk2.mak)

# eof
