# GL-binding for Linux32

# GL-Binding: 0=nothing (OpenGL provided from GUI-lib)
#             1=gtkgl  (GTK2: mit gtkgl-2.0)
#             2=GtkGlext  (nur mit VGUI = gtk2)
#             3=gtkglarea (nur mit VGUI = gtk2 / gtk+-2.0) (Mand.9.1)
# mdk2009-1: GtkGlext geht ned ..

GLBIND = 0
#GLBIND = 1

# cp /usr/local/lib/libgtkgl-2.0.a ../binLinux32/.



ifeq "$(GLBIND)" "0"
  SRCGLB = ../xa/GL_C-KIT.c
endif



ifeq "$(GLBIND)" "1"

  SRCGLB = ../xa/GL_glarea.c

  #GLBCP = -I/mnt/serv1/Devel/gtkgl-2.0
  GLBCP = -I/usr/include/gtkgl-2.0

  # dll referenzieren:
  GLBLIB = /usr/lib/libgtkgl-2.0.so.1

  # dll miteinbinden:
  #GLBLIB = /usr/lib/libgtkgl-2.0.a
endif



ifeq "$(GLBIND)" "2"
  SRCGLB = ../xa/GL_glext.c
  #glb_inc_dir = ../gtk_MS_216/include/gtkglext-1.0
  glb_inc_dir = /usr/include/gtkglext-1.0
  #GLBCP = -I$(glb_inc_dir)
  GLBCP = -I/usr/include/gtkglext-1.0 -I/usr/lib/gtkglext-1.0/include
  GLBLIB = /usr/lib/libgdkglext-x11-1.0.so.0 /usr/lib/libgtkglext-x11-1.0.so.0
endif






# EOF
