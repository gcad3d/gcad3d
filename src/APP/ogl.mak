# OpenGL - libs


ifeq "$(OS)" "MS32"
ogl_lib_dir = ../MS_ogl/lib
OGLLIB = $(ogl_lib_dir)/OpenGL32.lib $(ogl_lib_dir)/Glu32.lib
endif


ifeq "$(OS)" "Linux32"
OGLLIB = -lGLU -lGL
endif


ifeq "$(OS)" "Linux64"
OGLLIB = -lGLU -lGL
endif

# EOF
