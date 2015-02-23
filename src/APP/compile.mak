# compile nn.c -> nn.o
.c.o:
	@echo ====================== C $@ ========================
#	echo "VEROS=$(VEROS) VGUI=$(VGUI)"
	$(CC) -c $(CPFLG) $<
#	if test $(@D) != "../xa"; then mv $(@F) $@; fi
	mv -f $(@F) $(OUTDIR)/.
#	etags -f $(*F).tag $<
	rm -f ../APP/$(*F).tag
	ctags -f ../APP/$(*F).tag -IMemTab $<


.cpp.o:
	@echo ================= Cpp $@ ========================
	# CPPFLG not yet set
	$(CC) -c $(CPPFLG) $<
	mv -f $(@F) $(OUTDIR)/.
	rm -f ../APP/$(*F).tag
	ctags -f ../APP/$(*F).tag -IMemTab $<

# EOF
