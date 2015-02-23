#


OBJ1 = $(patsubst %.c,%.o, $(notdir $(SRC1)))
OBJG = $(patsubst %.c,%.o, $(notdir $(SRCG)))
OBJA = $(patsubst %.c,%.o, $(notdir $(SRCA)))
OBJOS = $(patsubst %.c,%.o, $(notdir $(SRCOS)))
OBJGUI = $(patsubst %.c,%.o, $(notdir $(SRCGUI)))
OBJGLB = $(patsubst %.c,%.o, $(notdir $(SRCGLB)))


VPATH = $(OUTDIR):../xa/:../wc:../exp/:../db/:../ci/:../ut/:../gr/:../lic/:../gui/:../gui_$(VGUI)/

# EOF
