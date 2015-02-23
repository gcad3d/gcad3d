# GL-binding for Linux64

# GL-Binding: 1=gtkgl  (GTK2: mit gtkgl-2.0)
#             2=GtkGlext  (nur mit VGUI = gtk2)
#             3=gtkglarea (nur mit VGUI = gtk2 / gtk+-2.0) (Mand.9.1)
# mdk2009-1: GtkGlext geht ned ..


# cp /usr/lib/libgtkgl-2.0.a ~/binLinux64/.

GLBIND = 0
#GLBIND = 3

# 2011-03-29 ATI/AMD-Driver does not work for ATI Radeon "HD 5400 Series"






#=========================================================================
ifeq "$(GLBIND)" "0"
  SRCGLB = ../xa/GL_C-KIT.c
endif



#=========================================================================
# GtkGlext
ifeq "$(GLBIND)" "2"

SRCGLB = ../xa/GL_glext.c

#glb_inc_dir = ../gtk_MS_216/include/gtkglext-1.0
#GLBCP = -I$(glb_inc_dir)
GLBCP = -I/usr/include/gtkglext-1.0 -I/usr/lib/gtkglext-1.0/include

#GLBLIB = $(glb_lib_dir)/gdkglext-win32-1.0.lib $(glb_lib_dir)/gtkglext-win32-1.0.lib
GLBLIB = /usr/lib/libgdkglext-x11-1.0.so.0 /usr/lib/libgtkglext-x11-1.0.so.0

endif


#=========================================================================
# gtkglarea
ifeq "$(GLBIND)" "3"

  SRCGLB = ../xa/GL_glarea.c

  #GLBCP = -I/mnt/serv1/Devel/gtkgl-2.0
  GLBCP = -I/usr/include/gtkgl-2.0

  #glb_lib_dir = ../gtk_MS_216/lib

  # dll referenzieren:
  #GLBLIB =../libgtkgl-2.0.so.1
  #GLBLIB = ../binLinux64/libgtkgl-2.0.a
  #GLBLIB = /usr/lib/libgtkgl-2.0.so.1
  GLBLIB = -lgtkgl-2.0

  # dll miteinbinden:
  GLBLIB = /usr/lib/libgtkgl-2.0.a


endif




# EOF
