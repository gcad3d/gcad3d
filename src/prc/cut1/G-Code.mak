# . ../options.sh && make -f ../prc/cut1/G-Code.mak
#

EXENAM  = G-Code


OBJ1 =\
  ../prc/cut1/G-Code.c\


# Sourcen; bei denen keine Abhaengigkeit erforderlich ist.
OBJ2 =\
  ../prc/cut1/pp_ut.c\


# get OUTDIR CC VGUI
#include ../options.mak


ADDLIBS = -lm

# Sourcefileliste fuers tagfile
SRC1 = ${OBJ1:.o=.c}
SRC2 = ${OBJ2:.o=.c}


CXFLG = -O0 -g3 -Wall

LKFLG = -lm

VPATH = $(gcad_dir_bin):../prc/cut1/


$(EXENAM): $(OBJ1) $(OBJ2)
	@echo "build $(EXENAM) -> $(gcad_dir_bin)"
	$(CC) -o $(EXENAM) $(OBJ1) $(OBJ2) $(LKFLG)
	ctags --excmd=number -f ut.tag $(SRC2)
	mv -f $(EXENAM) $(gcad_dir_bin)plugins/cut1/.


# vi holt fuer Alt f (grep) damit seine Sourcefileliste
SRCLST:
	@echo $(SRC1) $(SRC2)


.c.o:
	$(CC) -c $(CXFLG) $<
	mv $(@F) $@
#	if test $(@D) != "."; then mv $(@F) $@; fi
	ctags -f ../tags/$*.tag $<


# EOF
