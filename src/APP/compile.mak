# compile nn.c -> nn.o
.c.o:
	@echo ====================== C $@ ========================
#	echo "VEROS=$(VEROS) VGUI=$(VGUI)"
	@echo "VPATH="$(VPATH)
	$(CC) -c $(CPFLG) $<
#	if test $(@D) != "../xa"; then mv $(@F) $@; fi
	mv -f $(@F) $(gcad_dir_bin)/.
#	etags -f $(*F).tag $<
	rm -f ../tags/$(*F).tag
	ctags -f ../tags/$(*F).tag -IMemTab $<


.cpp.o:
	@echo ================= Cpp $@ ========================
	# CPPFLG not yet set
	$(CC) -c $(CPPFLG) $<
	mv -f $(@F) $(gcad_dir_bin)/.
	rm -f ../tags/$(*F).tag
	ctags -f ../tags/$(*F).tag -IMemTab $<

# EOF
