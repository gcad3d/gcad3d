# . ./devbase.sh && make -f gcInfo.mak

# update all gcad-INF-tagfiles

SRC1 = $(shell find ../inf/ -type f -iname "*.c")

DIRSRC1 := ../inf/
VPATH := ../inf/:../tags/


.SUFFIXES: .c .tag

TAG1 := $(patsubst %.c,%.tag, $(notdir $(SRC1)))


gcInfo: $(TAG1)
#	@echo $(SRC1)
#	@echo $(TAG1)


.c.tag:
	@echo ====================== compile $@ ========================
	rm -f ../tags/$(@F)
	ctags -f ../tags/$(@F) -IMemTab --c-kinds=dfstvx --tag-relative=yes --excmd=pattern $<
#	# etags -f $(*F).tag $<

# EOF
