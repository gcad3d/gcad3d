# create tag-files for INF_files (../inf/*)
#
# make -f inf_gcad3d.mak

#SRC1 = $(shell find ../inf/ -type f -iname "*.c")
SRC1 = $(shell find inf/ -type f -iname "*.c")

OBJ1 = $(patsubst %.c,%.tag, $(notdir $(SRC1)))
OBJ2 = $(addprefix ../tags/, $(OBJ1))

CMD = ctags -f $@ $<


#=============================================================
all: $(OBJ2)
	@echo " - inf_gcad3d.mak  done"


#=============================================================
#../tags/%.tag: ../inf/%.c
../tags/%.tag: inf/%.c
	@echo "===============inf_gcad3d.mak ctags $<"
	$(CMD)

# eof
